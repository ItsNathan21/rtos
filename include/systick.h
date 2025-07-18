#include <inttypes.h>

typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} systick_regset_t;

#define SYSTICK_BASE (systick_regset_t *) 0xE000E010

void systick_init(uint32_t period);
void systick_delay(uint32_t ticks);
