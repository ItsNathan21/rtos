#include "../include/handler.h"
#include "../include/uart.h"
#include "../include/nvic.h"
#include "../include/systick.h"
#include "../include/thread.h"

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

    uart_regset_t *uart = UART2_BASE;
    if (uart->SR & TXE) {
        USART2_tx_handler();
    }
    if (uart->SR & RXNE) {
        USART2_rx_handler();
    }
    change_interrupt_status(USART2_INT_NUM, CLEAR_PEND);
}

extern volatile uint32_t systick_ticks;

void systick_handler() {
    systick_ticks++;
    if (scheduler_ready) pend_pendsv();
}

void svc_C_handler(uint32_t *psp) {
    breakpoint;
}

