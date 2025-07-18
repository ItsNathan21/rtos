#include "../include/uart.h"
#include "../include/gpio.h"
#include "../include/rcc.h"
#include "../include/nvic.h"
#include "../include/stdbool.h"

#include "../include/breakpoint.h"

#define USART2_CLOCK_EN (1 << 17)

#define RXNEIE (1 << 5)
#define UE (1 << 13)
#define UART_8_BIT_WORD (1 << 12)
#define START_BITS (0b11 << 12)
#define BAUD_RATE (0x88)
#define TE (1 << 3)
#define RE (1 << 2)
#define RXNEIE (1 << 5)
#define TXEIE (1 << 7)

#define UART_BUFSIZE (16)
#define BACKSPACE (0x08)

typedef struct {
    uint8_t payload[UART_BUFSIZE];
    volatile uint32_t head;
    volatile uint32_t tail;
} uart_buf_t;

uart_buf_t uart_tx_buf = {.head = 0, .tail = 0};
uart_buf_t uart_rx_buf = {.head = 0, .tail = 0};

void uart_init(void) {
    // Setting pin A_3 as USART1_RX (receiving)
    gpio_init(GPIO_A, 3, MODE_ALT, OUTPUT_PUSH_PULL, OUTPUT_SPEED_VERY_HIGH, PUPD_NONE, ALT7);
    // Setting pin A_2 as USART1_TX (transmitting)
    gpio_init(GPIO_A, 2, MODE_ALT, OUTPUT_PUSH_PULL, OUTPUT_SPEED_VERY_HIGH, PUPD_NONE, ALT7);

    struct rcc_reg_map *rcc = RCC_BASE;
    rcc->apb1_enr |= USART2_CLOCK_EN;

    uart_regset_t *uart = UART2_BASE;
    uart->CR1 &= ~UART_8_BIT_WORD; // sets UART to use 8 bit words
    uart->CR1 |= TE; // enable UART transmission 
    uart->CR1 |= RE;
    uart->CR1 |= RXNEIE; // enable receiving interrupts
    uart->CR2 &= ~START_BITS; //  use 1 stop bit
    uart->BRR = BAUD_RATE;
    uart->CR1 |= UE; // enables UART transmission and receiving

    change_interrupt_status(USART2_INT_NUM, ENABLE_INTERRUPT);
}

static bool uart_buf_full(uart_buf_t *uart_buf) {
    uint32_t next_head = (uart_buf->head + 1) % UART_BUFSIZE;
    return next_head == uart_buf->tail;
}

static bool uart_buf_empty(uart_buf_t *uart_buf) {
    return uart_buf->head == uart_buf->tail;
}

static bool uart_push(uart_buf_t *uart_buf, uint8_t c) {
    if (uart_buf_full(uart_buf)) return false;
    uart_buf->payload[uart_buf->head] = c;
    uart_buf->head = (uart_buf->head + 1) % UART_BUFSIZE;

    // Update interrupts if needed
    uart_regset_t *uart = UART2_BASE;
    if (uart_buf == &uart_tx_buf && !(uart->CR1 & TXEIE)) {
        uart->CR1 |= TXEIE; // enable transmit interrupts
    }
    if (uart_buf == &uart_rx_buf && uart_buf_full(uart_buf)) {
        uart->CR1 &= ~RXNEIE; // disable receiving interrupts
    }
    return true;
}

static bool uart_pop(uart_buf_t *uart_buf, uint8_t *c) {
    if (uart_buf_empty(uart_buf)) return false;
    *c = uart_buf->payload[uart_buf->tail];
    uart_buf->tail = (uart_buf->tail + 1) % UART_BUFSIZE;

    uart_regset_t *uart = UART2_BASE;
    if (uart_buf == &uart_tx_buf && uart_buf_empty(uart_buf)) {
        uart->CR1 &= ~TXEIE; // disable transmit interrupts
    }
    return true;
}

void USART2_tx_handler(void) {
    uint8_t c;
    uart_regset_t *uart = UART2_BASE;
    if (uart_pop(&uart_tx_buf, &c)) {
        uart->DR = c;
    }
} 

void USART2_rx_handler(void) {
    uart_regset_t *uart = UART2_BASE;
    uint8_t c = uart->DR;
    uart_push(&uart_rx_buf, c);
}

void uart_putchar(uint8_t c) {
    if (c == BACKSPACE) {
        while (!uart_push(&uart_tx_buf, '\b'));
        while (!uart_push(&uart_tx_buf, ' '));
        while (!uart_push(&uart_tx_buf, '\b'));
    }
    while (!uart_push(&uart_tx_buf, c));
}

void uart_getchar(uint8_t *c) {
    while(!uart_pop(&uart_rx_buf, c));
}

void uart_puts(char *s) {
    while (*s) {
        uart_putchar(*s);
        s++;
    }
} 

