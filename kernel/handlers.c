#include "../include/handler.h"
#include "../include/uart.h"
#include "../include/nvic.h"
#include "../include/systick.h"
#include "../include/thread.h"
#include "../include/svc.h"

void nmi() {
    breakpoint;
}

void hard_fault() {
    breakpoint;
}

void mm_fault() {
    breakpoint;
}

void bus_fault() {
    breakpoint;
}

void usage_fault() {
    breakpoint;
}


void USART2_IRQHandler() {
    #define TXE (1 << 7)
    #define RXNE (1 << 5)

    // int16_t state = save_interrupt_state_and_disable();
    uart_regset_t *uart = UART2_BASE;
    if (uart->SR & TXE) {
        USART2_tx_handler();
    }
    if (uart->SR & RXNE) {
        USART2_rx_handler();
    }
    change_interrupt_status(USART2_INT_NUM, CLEAR_PEND);
    // restore_interrupt_state(state);
}

extern volatile uint32_t systick_ticks;


void systick_handler() {
    systick_ticks++;
    pend_pendsv();
}

void svc_C_handler(uint32_t *psp) {
    uint32_t svc_number = *(uint32_t *)(*(psp + 6) - 2) & 0xff;
 
    uint32_t r0 = psp[0];
    uint32_t r1 = psp[1];
    // uint32_t *r2 = psp + 2;
    // uint32_t *r3 = psp + 3;
    // uint32_t *input_5 = psp + 8;

    switch (svc_number) {
        case nthread_create_svc: {
            bool ret = _nthread_create((void *)r0, (void *)r1);
            *(&r0) = (uint32_t)ret;
            break;
        }
        case initialize_threads_svc: {
            bool ret = _initialize_threads(r0);
            *(&r0) = (uint32_t)ret;
            break;
        }
        case nthread_kill_svc: {
            _nthread_kill(r0);
            break;
        }
        case nthread_run_svc: {
            bool ret = _nthread_run(r0);
            *(&r0) = (uint32_t)ret;
            break;
        }
        case start_svc: {
            _start();
            break;
        }
    }

    
}

