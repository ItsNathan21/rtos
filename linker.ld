/* Linker script for STM32F401 Nucleo
 * Flash: 512K @ 0x08000000
 * RAM:   96K  @ 0x20000000
 */

ENTRY(reset)

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
}

PROVIDE(__msp_stack_top = ORIGIN(RAM) + LENGTH(RAM));

SECTIONS
{
  /* Interrupt Vector Table (84 entries * 4 bytes = 336 bytes) */
   .isr_vector 0x08000000 : {
        KEEP(*(.isr_vector))  /* MUST keep */
        . = ALIGN(4);
        _eisr_vector = .;
    } > FLASH

  /* Unified .text section (includes reset, kernel, user, and misc code) */
  .text :
  {
    _text_start = .;

    KEEP(*(.text.reset))          /* reset handler */
    KEEP(*(.text.kernel*))        /* kernel code */
    KEEP(*(.text.user*))          /* user-mode stubs and code */
    KEEP(*(.swi_stub))            /* SVC stubs if present */
    *(.text*)                     /* other generic text */

    _text_end = .;
  } > FLASH

  /* Read-only data */
  .rodata :
  {
    . = ALIGN(1024);
    _k_rodata = .;
    *(.rodata.kernel*)
    . = ALIGN(1024);
    _u_rodata = .;
    *(.rodata.user*)
    *(.rodata*)
    _u_erodata = .;
  } > FLASH

  _erodata = .;

  /* Initialized Data — copied from Flash to RAM */
  .data : AT(_erodata)
  {
    . = ALIGN(4);
    _k_data = .;
    *(.data.kernel*)
    . = ALIGN(1024);
    _u_data = .;
    *(.data.user*)
    *(.data*)
    _u_edata = .;
  } > RAM

  /* Uninitialized Data */
  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    _bss_start = .;
    _k_bss = .;
    *(.bss.kernel*) *(.bss*)
    *(COMMON)
    . = ALIGN(1024);
    _u_bss = .;
    *(.bss.user*) *(COMMON)
    _u_ebss = .;
  } > RAM

  _bss_size = _u_ebss - _bss_start;
  _data_size = _u_edata - _k_data;

  /* Heap and stack layout */
  . = ALIGN(8 * 1024);
  __heap_low = .;
  . = . + (4 * 1024);
  __heap_top = .;

  __psp_stack_bottom = .;
  . = . + (2 * 1024);
  __psp_stack_top = .;

  __msp_stack_bottom = .;
  . = . + (2 * 1024);
  __msp_stack_top = .;

  /* Extra kernel heap region */
  __kheap_low_0 = .;
  . = . + (8 * 1024);
  __kheap_top_0 = .;

  /* User stack space */
  . = ALIGN(32 * 1024);
  __thread_u_stacks_low = .;
  . = . + (32 * 1024);
  __thread_u_stacks_top = .;

  /* Kernel stack space */
  __thread_k_stacks_low = .;
  . = . + (32 * 1024);
  __thread_k_stacks_top = .;

  end = .;
}