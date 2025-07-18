#include <inttypes.h>


typedef struct {
    volatile uint32_t FPCCR;
    volatile uint32_t FPCAR;
    volatile uint32_t FPDSCR;
} fpu_regset_t;

#define FPU_BASE (fpu_regset_t *) 0xE000EF34

void fpu_init(void);
