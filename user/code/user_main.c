#include "../../include/uart.h"
#include "../include/syscalls.h"
#include "../../include/breakpoint.h"

uint32_t val = 1;

void thread1(void) {
    while (1) {
        uart_puts("Hello from thread 1\n");
        // breakpoint;
    }
}

void thread2(void *val) {
    uint32_t i = *(uint32_t *)val;
    while(1) {
        uart_puts("Hello from thread 2\n");
    }
}

void thread3() {
    while(1)
        uart_puts("Hello from thread 3!\n");
}

void thread4() {
    while(1)
        uart_puts("Hello from thread 4!\n");
}

int __init__() {
    uart_puts("Hello from user code!\n");
    initialize_threads(4);
    nthread_create(thread1, (void *)0);
    nthread_create(thread2, (void *)val);
    nthread_create(thread3, (void *)0);
    nthread_create(thread4, (void *)0);
    nthread_run(1);
    nthread_run(2);
    nthread_run(3);
    nthread_run(4);

    start();
    while (1) {
        uart_puts("Hello from __init__\n");
    }
    return 1;
}