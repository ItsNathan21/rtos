#include <inttypes.h>
#include "../include/systick.h"

#define TICKINT (1 << 1)
#define ENABLE (1 << 0)
#define CLKSOURCE2 (1 << 2)

#define MILISECOND_TICKS (16000000 / 1000)

volatile uint32_t systick_ticks;

void systick_init(uint32_t period) {
    systick_regset_t *systick = SYSTICK_BASE;
    systick->CTRL |= TICKINT;
    systick->CTRL |= CLKSOURCE2;

    systick->LOAD = MILISECOND_TICKS * period;

    systick->CTRL |= ENABLE;
}
void systick_delay(uint32_t ticks) {
    uint32_t target_ticks = systick_ticks + ticks;
    while (systick_ticks < target_ticks);
}
