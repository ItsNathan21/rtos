#include <inttypes.h>

typedef struct {
    volatile uint32_t CPUID;
    volatile uint32_t ICSR;
    volatile uint32_t VTOR;
    volatile uint32_t AIRCR;
    volatile uint32_t SCR;
    volatile uint32_t CCR;
    volatile uint32_t SHPR1;
    volatile uint32_t SHPR2;
    volatile uint32_t SHPR3;
    volatile uint32_t SHCSR;
    volatile uint32_t CFSR;
    volatile uint32_t MMSR;
    volatile uint32_t BRSR;
    volatile uint32_t UFSR;
    volatile uint32_t HRSR;
    volatile uint32_t MMAR;
    volatile uint32_t BFAR;
    volatile uint32_t AFSR;
} system_control_block_regset_t;


#define SCB_BASE (system_control_block_regset_t *) 0xE000ED00
