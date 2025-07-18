#include "../../include/uart.h"


int __init__() {
    uart_puts("Hello from user code!\n");
    // char x[10];
    // char *c = __file_top;
    // *__file_top = 0x69;
    while (1);
    return 1;
}