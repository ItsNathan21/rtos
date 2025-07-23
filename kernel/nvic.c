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

int16_t save_interrupt_state_and_disable(void) {
  int16_t result;
  int16_t disable_constant = 1;
  __asm volatile("mrs %0, PRIMASK" : "=r"(result));
  __asm volatile("msr PRIMASK, %0" : "=r"(disable_constant));
  __asm volatile("cpsid i");
  return result;
}

void restore_interrupt_state(int32_t state) {
  __asm volatile("msr PRIMASK, %0" : "=r"(state));
}

