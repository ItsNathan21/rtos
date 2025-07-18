

.cpu cortex-m4
.syntax unified
.section .isr_vector
.thumb

.global isr_vector, "a"
isr_vector:
    .word   __msp_stack_top     /* stack top address */
    .word   reset            /* 1 Reset */
    .word   nmi             /* 2 NMI */
    .word   hard_fault        /* 3 Hard Fault */
    .word   mm_fault          /* 4 MM Fault */
    .word   bus_fault         /* 5 Bus Fault */
    .word   usage_fault       /* 6 Usage Fault */
    .word   spin                /* 7 RESERVED */
    .word   spin                /* 8 RESERVED */
    .word   spin                /* 9 RESERVED*/
    .word   spin                /* 10 RESERVED */
    .word   spin               /* 11 SV call */
    .word   spin                /* 12 Debug reserved */
    .word   spin                /* 13 RESERVED */
    .word   pendsv_handler           /* 14 PendSV */
    .word   systick_handler      /* 15 SysTick */
    .word   spin                /* 16 IRQ0 Window Watchdog Interrupt */
    .word   spin                /* 17 IRQ1 PVD */
    .word   spin                /* 18 IRQ2 TAMPER */
    .word   spin                /* 19 IRQ3 RTC   */
    .word   spin                /* 20 IRQ4 FLASH   */
    .word   spin                /* 21 IRQ5 RCC */
    .word   spin                /* 22 IRQ6 EXTI0 */
    .word   spin                /* 23 IRQ7 EXTI1  */
    .word   spin                /* 24 IRQ8 EXTI2   */
    .word   spin                /* 25 IRQ9 EXTI3 */
    .word   spin                /* 26 IRQ10 EXTI4 */
    .word   spin                /* 27 IRQ11 DMA1_Channel1   */
    .word   spin                /* 28 IRQ12 DMA1_Channel2   */
    .word   spin                /* 29 IRQ13 DMA1_Channel3 */
    .word   spin                /* 30 IRQ14 DMA1_Channel4 */
    .word   spin                /* 31 IRQ15 DMA1_Channel5   */
    .word   spin                /* 32 IRQ16 DMA1_Channel6   */
    .word   spin                /* 33 IRQ17 DMA1_Channel7 */
    .word   spin                /* 34 IRQ18 ADC1_2 */
    .word   spin                /* 35 IRQ19 CAN1_TX   */
    .word   spin                /* 36 IRQ20 CAN1_TX0   */
    .word   spin                /* 37 IRQ21 CAN1_RX1 */
    .word   spin                /* 38 IRQ22 CAN1_SCE */
    .word   spin                /* 39 IRQ23 EXTI9_5   */
    .word   spin                /* 40 IRQ24 TIM1_BRK   */
    .word   spin                /* 41 IRQ25 TIM1_UP */
    .word   spin                /* 42 IRQ26 TIM1_TRG_COM */
    .word   spin                /* 43 IRQ27 TIM1_CC   */
    .word   spin                /* 44 IRQ28 TIM2   */
    .word   spin                /* 45 IRQ29 TIM3 */
    .word   spin                /* 46 IRQ30 TIM5 */
    .word   spin                /* 47 IRQ31 I2C1_EV   */
    .word   spin                /* 48 IRQ32 I2C1_ER   */
    .word   spin                /* 49 IRQ33 I2C2_EV */
    .word   spin                /* 50 IRQ34 I2C2_ER */
    .word   spin                /* 51 IRQ35 SPI1   */
    .word   spin                /* 52 IRQ36 SPI2   */
    .word   spin                /* 53 IRQ37 USART1 */
    .word   USART2_IRQHandler   /* 54 IRQ38 USART2 */
    .word   spin                /* 55 IRQ39 USART3   */
    .word   spin                /* 56 IRQ40 EXTI15_10   */
    .word   spin                /* 57 IRQ41 RTCAlarm */
    .word   spin                /* 58 IRQ42 OTG_FS_WKUP */
    .word   spin                /* 59 IRQ43 RESERVED   */
    .word   spin                /* 60 IRQ44 RESERVED   */
    .word   spin                /* 61 IRQ45 RESERVED */
    .word   spin                /* 62 IRQ46 RESERVED */
    .word   spin                /* 63 IRQ47 RESERVED   */
    .word   spin                /* 64 IRQ48 RESERVED   */
    .word   spin                /* 65 IRQ49 RESERVED */
    .word   spin                /* 66 IRQ50 TIM5 */
    .word   spin                /* 67 IRQ51 SPI3   */
    .word   spin                /* 68 IRQ52 UART4   */
    .word   spin                /* 69 IRQ53 UART5 */
    .word   spin                /* 70 IRQ54 TIM6 */
    .word   spin                /* 71 IRQ55 TIM7   */
    .word   spin                /* 72 IRQ56 DMA2_Channel1   */
    .word   spin                /* 73 IRQ57 DMA2_Channel2 */
    .word   spin                /* 74 IRQ58 DMA2_Channel3 */
    .word   spin                /* 75 IRQ59 DMA2_Channel4   */
    .word   spin                /* 76 IRQ60 DMA2_Channel5   */
    .word   spin                /* 77 IRQ61 ETH */
    .word   spin                /* 78 IRQ62 ETH_WKUP */
    .word   spin                /* 79 IRQ63 CAN2_TX   */
    .word   spin                /* 80 IRQ64 CAN2_RX0  */
    .word   spin                /* 81 IRQ65 CAN2_RX1 */
    .word   spin                /* 82 IRQ66 CAN2_SCE */
    .word   spin                /* 83 IRQ67 OTG_FS   */

.thumb_func
.global spin
spin:
    bkpt

.section .text, "ax", %progbits

.thumb_func
.global reset
reset:
  LDR r1, =_erodata @ load the starting data pointer into r1
  LDR r2, =_k_data  @ load the starting copying pointer into r2
  LDR r3, =_u_edata @ load the ending copying pointer into r3

  subs r4, r3, r2   @ load the size of data into r4
  mov r5, #0        @ zero out r5 to act as an index
  cmp r4, #0
  beq zero_bss

copy_loop:
  ldr r6, [r1, r5, lsl #2] @ r6 := flash[base + index] 
  str r6, [r2, r5, lsl #2] @ SRAM[base + index] := r6
  adds r5, r5, #1           @ increase index
  subs r0, r4, r5           @ checking the r5 index
  cmp r0, #0
  bgt copy_loop

zero_bss:
  LDR r1, =_bss_start 
  LDR r2, =_u_ebss

  subs r3, r2, r1   @ load the length of bss area into r3
  cmp r3, #0
  beq =kernel_main
  mov r4, #0        @ zero out r4 to store zero everywhere
  mov r5, #0        @ zero out r5 to act as an index

bss_loop:
  str r4, [r1, r5, lsl #2]  @ store r4 (0x0) into .bss area
  adds r5, r5, #1           @ increase index
  subs r0, r3, r5           @ check if r5 is too big
  cmp r0, #0
  bgt bss_loop

  ldr r0, =kernel_main
  bx r0

stall:
    bkpt

.thumb_func
.global pendsv_handler
pendsv_handler:
  mrs r0, psp
  b pendsv_C_handler
