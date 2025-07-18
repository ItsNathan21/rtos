/* Linker script for STM32F401 Nucleo
 * Flash: 512K @ 0x08000000
 * RAM:   96K  @ 0x20000000
 */

ENTRY(reset)

MEMORY
{
  FLASH_CODE (rx) : ORIGIN = 0x08000000, LENGTH = 512K     /* 0x78000 */
  RAM        (rwx): ORIGIN = 0x20000000, LENGTH = 96K
}

PROVIDE(__msp_stack_top = ORIGIN(RAM) + LENGTH(RAM));

SECTIONS
{
  /* Interrupt Vector Table (84 entries * 4 bytes = 336 bytes) */
  .isr_vector ORIGIN(FLASH_CODE) : {
    KEEP(*(.isr_vector))
    . = ALIGN(4);
    _eisr_vector = .;
  } > FLASH_CODE

  /* Unified .text section */
  .text :
  {
    _text_start = .;

    KEEP(*(.text.reset))
    KEEP(*(.text.kernel*))
    KEEP(*(.text.user*))
    KEEP(*(.swi_stub))
    *(.text*)

    _text_end = .;
  } > FLASH_CODE

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
  } > FLASH_CODE

  _erodata = .;

  /* Initialized Data */
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

  __kheap_low_0 = .;
  . = . + (8 * 1024);
  __kheap_top_0 = .;

  . = ALIGN(32 * 1024);
  __thread_u_stacks_low = .;
  . = . + (32 * 1024);
  __thread_u_stacks_top = .;

  __thread_k_stacks_low = .;
  . = . + (32 * 1024);
  __thread_k_stacks_top = .;

  end = .;
}