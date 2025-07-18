#include <inttypes.h>
#include "../include/stdbool.h"

typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} uart_regset_t;

#define UART2_BASE (uart_regset_t *) 0x40004400
#define USART2_INT_NUM (38)

void uart_init(void);
void USART2_tx_handler(void);
void USART2_rx_handler(void);
void uart_putchar(uint8_t c);
void uart_getchar(uint8_t *c);
void uart_puts(char *s);

