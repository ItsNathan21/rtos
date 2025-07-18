#include <inttypes.h>
#include "../include/fpu.h"

#define LSPEN (1 << 30)
#define ASPEN (1 << 31)

void fpu_init(void) {
    fpu_regset_t *fpu = FPU_BASE;
    fpu->FPCCR &= ~LSPEN;
    fpu->FPCCR &= ASPEN;
}