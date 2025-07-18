#include "../include/breakpoint.h"
#include "../include/nvic.h"

void change_interrupt_status(int32_t num, interrupt_state_t state) {
    nvic_register_set_t *nvic = 0;
    uint32_t bit = num % NVIC_REG_SIZE;
    uint32_t reg_idx = num / NVIC_REG_SIZE;
    switch (state) {
        case ENABLE_INTERRUPT: {
            nvic = NVIC_SET_REG;
            break;
        }
        case DISABLE_INTERRUPT: {
            nvic = NVIC_CLR_REG;
            break;
        }
        case PEND_INTERRUPT: {
            nvic = NVIC_SET_PENDING_REG;
            break;
        }
        case CLEAR_PEND: {
            nvic = NVIC_CLR_PENDING_REG;
            break;
        }
        
    }
    nvic->reg[reg_idx] |= (0x1 << bit);
}


