#include "../../include/uart.h"
#include "../include/syscalls.h"




int __init__() {
    uart_puts("Hello from user code!\n");
    char x[10];
    // char *c = __file_top;
    // *__file_top = 0x69;
    nthread_create();
    while (1);
    return 1;
}