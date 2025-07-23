#include "../include/stdbool.h"
#include <stdint.h>

#define MAX_THREADS (32)
#define NULL (void *)0

typedef enum {
    UNINITIALIZED = 0,
    READY = 1, 
    RUNNING = 2
} task_state_t;

typedef struct {
    uint32_t R0;
    uint32_t R1;
    uint32_t R2;
    uint32_t R3;
    uint32_t R12;
    uint32_t LR;
    uint32_t PC;
    uint32_t xPSR;
} psp_stack_exception_t;

typedef struct {
    psp_stack_exception_t *psp;
    uint32_t R4;
    uint32_t R5;
    uint32_t R6;
    uint32_t R7;
    uint32_t R8;
    uint32_t R9;
    uint32_t R10;
    uint32_t R11;
    uint32_t LR;
} task_context_t;

typedef struct {
    task_context_t *context;
    intptr_t psp_top;
    intptr_t msp_top;
    task_state_t state;
    bool svc_status;
} task_t;

typedef struct {
    void *fn;
    void *vargp;
} task_info_t;

void pend_pendsv(void);

void clr_pend_pendsv(void);

task_context_t *pendsv_C_handler(task_context_t *context);

bool _initialize_threads(uint32_t threadnums);

bool _nthread_create(void *fn, void *vargp); 

void _nthread_kill(uint32_t threadnum);


bool _nthread_run(uint32_t threadnum);

void _start(void);
