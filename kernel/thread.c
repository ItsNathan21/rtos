#include "../include/scb.h"
#include "../include/breakpoint.h"

uint32_t num_threads, curr_thread;

void pend_pendsv() {
    #define PENDSVSET (1 << 28)
    system_control_block_regset_t *scb = SCB_BASE;
    scb->ICSR |= PENDSVSET;
}

void clr_pend_pendsv() {
    #define PENDSVCLR (1 << 27)
    system_control_block_regset_t *scb = SCB_BASE;
    scb->ICSR |= PENDSVCLR;
    // test
}

void pendsv_C_handler(uint32_t *psp) {
    breakpoint;
}

void nthread_initialize(void) {
    num_threads = 2;
    curr_thread = 
}

uint32_t _nthread_create(void (*fn(void)), void *vargp) {

}

