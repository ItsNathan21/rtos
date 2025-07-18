
.cpu cortex-m4
.syntax unified
.thumb

.section .text

.global __pre__init__
.thumb_func
__pre__init__:
    ldr r0, =__psp_stack_top
    msr psp, r0
    mov r0, #3
    msr control, r0
    isb
    b __init__