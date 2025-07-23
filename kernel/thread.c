#include "../include/scb.h"
#include "../include/breakpoint.h"
#include "../include/thread.h"
#include "../include/systick.h"
#include "../include/uart.h"


#define xPSR_INIT (0x1000000)
#define THREAD_PERIOD_MS (50)
#define UNAVAILABLE (0)

static uint32_t num_threads = 1;
static uint32_t num_threads_running = 1;
static uint32_t curr_thread = 0;
static uint32_t max_threads,
                user_stack_size, kernel_stack_size;
static task_info_t thread_info[MAX_THREADS + 1];
static task_t threads[MAX_THREADS + 1];

static intptr_t available_msp[MAX_THREADS] = {UNAVAILABLE};
static intptr_t available_psp[MAX_THREADS] = {UNAVAILABLE};
static uint32_t available_threads = ~0;


extern char __thread_k_stacks_top, __thread_k_stacks_low, 
            __thread_u_stacks_top, __thread_u_stacks_low,
            __msp_stack_top, __psp_stack_top;
extern volatile uint32_t systick_ticks;

extern void nthread_kill(void);
extern int terminal_main(void);

bool scheduler_ready = false;

static void set_svc_status(bool status) {
    #define SVC_ACTIVE (1 << 7)
    system_control_block_regset_t *scb = SCB_BASE;
    scb->SHCSR &= ~SVC_ACTIVE;
    scb->SHCSR |= (status << 7);
}

static bool get_svc_status(void) {
    #define SVC_ACTIVE (1 << 7)
    system_control_block_regset_t *scb = SCB_BASE;
    if (scb->SHCSR & SVC_ACTIVE) return true;
    return false;
}

void pend_pendsv() {
    #define PENDSVSET (1 << 28)
    system_control_block_regset_t *scb = SCB_BASE;
    scb->ICSR |= PENDSVSET;
}

void clr_pend_pendsv() {
    #define PENDSVCLR (1 << 27)
    system_control_block_regset_t *scb = SCB_BASE;
    scb->ICSR |= PENDSVCLR;
}

static intptr_t find_available_sp(intptr_t sp[MAX_THREADS]) {
    for (uint32_t i = 0; i < max_threads; i++) {
        if (sp[i] != UNAVAILABLE) {
            intptr_t ptr = sp[i];
            sp[i] = UNAVAILABLE;
            return ptr;
        }
    }
    return UNAVAILABLE;
}

static void add_available_sp(intptr_t sp[MAX_THREADS], intptr_t ptr) {
    for (uint32_t i = 0; i < max_threads; i++) {
        if (sp[i] == UNAVAILABLE) {
            sp[i] = ptr;
            return;
        }
    }
}

static bool is_available_thread(uint32_t i) {
    return available_threads & (1 << i) ? true : false;
}

static uint32_t find_available_thread(void) {
    uint32_t i = 0;
    for (; i < max_threads; i++) {
        if (is_available_thread(i)) {
            available_threads &= ~(1 << i);
            return i;
        }
    }
    return i;
}

static void add_available_thread(uint32_t threadnum) {
    available_threads |= (1 << threadnum);
}


static uint32_t get_next_running_thread(void) {
    uint32_t threadnum = (curr_thread + 1) % max_threads;
    uint32_t cnt = 0;
    uint32_t i;
    for (i = 0; cnt < threadnum; i++) {
        if (!is_available_thread(i)) {
            if (cnt == threadnum) return i;
            cnt++;
        }
    }
    return i;
}

task_context_t *pendsv_C_handler(task_context_t *context) {

    uint32_t new_thread = get_next_running_thread();
    if (new_thread == curr_thread) return context;

    bool svc_status = get_svc_status();
    task_t *old_task = &threads[curr_thread];
    task_t *new_task = &threads[new_thread];
    old_task->context = context;
    old_task->state = READY;
    old_task->svc_status = svc_status;
    set_svc_status(new_task->svc_status);
    new_task->state = RUNNING;
    curr_thread = new_thread;
    return new_task->context;
}

static void init_registers(uint32_t thread_num, void *fn, void *vargp) {
    task_t *task = &threads[thread_num];
    task->context->LR = 0xFFFFFFFD;
    task->context->psp->LR = (uint32_t)(&nthread_kill);
    task->context->psp->xPSR = xPSR_INIT;
    task->context->psp->PC = (uint32_t)fn;
    task->context->psp->R0 = (uint32_t)vargp;
}

bool _nthread_run(uint32_t threadnum) {
    if (num_threads_running >= (max_threads + 1)) return false;
    uint32_t thread_idx = find_available_thread();
    task_t *task = &threads[thread_idx];
    task_info_t *info = &thread_info[threadnum];
    task->context = (task_context_t *)(find_available_sp(available_msp));
    task->context->psp = (psp_stack_exception_t *)(find_available_sp(available_psp));
    task->msp_top = task->context;
    task->psp_top = task->context->psp;
    task->state = READY;
    init_registers(thread_idx, info->fn, info->vargp);
    return true;
}

bool _initialize_threads(uint32_t threadnums) {
    if (threadnums > MAX_THREADS) return false;
    max_threads = threadnums + 1;
    user_stack_size = (uintptr_t)(&__thread_u_stacks_top) - (uintptr_t)(&__thread_u_stacks_low);
    kernel_stack_size = (uintptr_t)(&__thread_k_stacks_top) - (uintptr_t)(&__thread_k_stacks_low);
    uint32_t i_user_stack_size = user_stack_size / max_threads;
    uint32_t i_kernel_stack_size = kernel_stack_size / max_threads;
    for (uint32_t i = 0; i < max_threads; i++) {

        #define ALIGN_DOWN(addr, align) ((addr) & ~((align) - 1))

        intptr_t raw_kstack = (intptr_t)&__thread_k_stacks_top - (i * i_kernel_stack_size);
        intptr_t raw_ustack = (intptr_t)&__thread_u_stacks_top - (i * i_user_stack_size);

        intptr_t kstack = ALIGN_DOWN(raw_kstack - sizeof(task_context_t), 8);
        intptr_t ustack = ALIGN_DOWN(raw_ustack - sizeof(psp_stack_exception_t), 8);
        add_available_sp(available_msp, kstack);
        add_available_sp(available_psp, ustack);
    }

    task_info_t *info = &thread_info[0];
    info->fn = &terminal_main;
    info->vargp = NULL;
    _nthread_run(0);
    init_registers(0, &terminal_main, NULL);
    return true;
}

bool _nthread_create(void *fn, void *vargp) {
    if (num_threads >= max_threads) return false;
    task_info_t *task = &thread_info[num_threads++];
    task->fn= fn;
    task->vargp = vargp;
    return true;
}   

void _nthread_kill(uint32_t threadnum) {
    task_t *task = &threads[threadnum];
    if (num_threads_running <= 1 || task->state == UNINITIALIZED || threadnum == 0) return;
    num_threads_running--;
    task->state = UNINITIALIZED;
    add_available_sp(available_msp, task->msp_top);
    add_available_sp(available_psp, task->psp_top);
    add_available_thread(threadnum);
}


void _start(void) {
    systick_init(THREAD_PERIOD_MS);
    pend_pendsv();
}

