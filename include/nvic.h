#include <stdint.h>

typedef struct {
    volatile uint32_t reg[8];
} nvic_register_set_t;

#define NVIC_SET_REG (nvic_register_set_t *) 0xE000E100
#define NVIC_CLR_REG (nvic_register_set_t *) 0xE000E180
#define NVIC_SET_PENDING_REG (nvic_register_set_t *) 0xE000E200
#define NVIC_CLR_PENDING_REG (nvic_register_set_t *) 0xE000E280

#define NVIC_REG_SIZE 32

typedef enum {
    ENABLE_INTERRUPT, 
    DISABLE_INTERRUPT, 
    PEND_INTERRUPT,
    CLEAR_PEND
} interrupt_state_t;

void change_interrupt_status(int32_t num, interrupt_state_t state);

int16_t save_interrupt_state_and_disable(void);

void restore_interrupt_state(int32_t state);
