#include "../include/stdbool.h"
#include <stdint.h>

bool scheduler_ready = false;

#define MAX_THREADS (64)

typedef struct {
    uint32_t R0;
    uint32_t R1;
    uint32_t R2;
    uint32_t R3;
    uint32_t R12;
    uint32_t LR;
    uint32_t PC;
    uint32_t xPSR;
} psp_stack_exception_t;

typedef struct {
    psp_stack_exception_t *psp;
    uint32_t R4;
    uint32_t R5;
    uint32_t R6;
    uint32_t R7;
    uint32_t R8;
    uint32_t R9;
    uint32_t R10;
    uint32_t R11;
    uint32_t lr;
} task_context_t;

typedef struct {
    int32_t pid;
    int32_t parent_pid;



    task_context_t *context;
} task_t;

task_t threads[MAX_THREADS];

void pend_pendsv(void);

void clr_pend_pendsv(void);

void pendsv_C_handler(void);
