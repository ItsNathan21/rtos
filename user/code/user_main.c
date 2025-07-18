#include "../../include/uart.h"


int __init__() {
    uart_puts("Hello from user code!\n");
    char x[10];
    return 1;
}