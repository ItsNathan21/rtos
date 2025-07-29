#include <stdint.h>
#include "../include/stdbool.h"
#include "../include/nvic.h"
#include "../include/breakpoint.h"
#include "../include/uart.h"
#include "../include/systick.h"
#include "../include/flash.h"

int32_t FibonacciNumbers[10];

uint8_t data[] = "This is the beginning of sector 7";


bool test_zeroing_bss(void) {
    for (int32_t i = 0; i < 10; i++) {
        if (FibonacciNumbers[i] != 0) {
            breakpoint;
        }
    }
    return true;
}

extern void __pre__init__(void);

int kernel_main() {

    if (test_zeroing_bss() == false) return 4;

    uart_init();
    // flash_unlock();

    __pre__init__();
    uint8_t c;
    while (1) {
        uart_getchar(&c);
        // systick_delay(1);
        uart_putchar(c);
    }
    breakpoint;
    return 0;
}