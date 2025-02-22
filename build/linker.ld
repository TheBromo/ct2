/* https://github.com/tmdarwen/STM32/blob/master/STM32F411/ArmCortexM4Analysis/LinkerScript.ld */
/* https://github.com/charleskorn/stm32f4-project-template/blob/master/lib/stm32f4xx/src/stm32f407vg.ld */

MEMORY
{
  ROM (rx): ORIGIN = 0x08000000, LENGTH = 1024K
  RAM (rwx): ORIGIN = 0x20000000, LENGTH = 128K
}

__rom_start__   = ORIGIN(ROM);
__rom_size__    = LENGTH(ROM);

__ram_start__   = ORIGIN(RAM);
__ram_size__    = LENGTH(RAM);
__ram_end__     = __ram_start__ + __ram_size__;

__stack_end__   = __ram_end__; /* top of ram */
__stack_size__  = 16K;
__stack_start__ = __stack_end__ - __stack_size__;

__heap_start__  = __bss_end__; /* between bss and stack */
__heap_end__    = __stack_start__;

end             = __heap_start__; /* used in sbrk.c */

ENTRY(Reset_Handler)

SECTIONS
{
  . = 0;

  /* program code */
  .text : {
    . = ALIGN(4);

    KEEP(*(.vectors)) /* startup vector table */
    *(.text)
    *(.text*)
    
    KEEP(*(.rodata))
    KEEP(*(.rodata*))

    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)

    . = ALIGN(4);
    __text_end__ = .;
  } >ROM

  /* initialized data sections */
  .data : {
    . = ALIGN(4);
    __data_beg__ = .; /* used in crt0.S */
    *(.data)
    *(.data*)
    . = ALIGN(4);
    __data_end__ = .; /* used in crt0.S */
  } >RAM AT > ROM

  /* uninitialized data sections */
  .bss (NOLOAD) : {
    . = ALIGN(4);
    __bss_beg__ = .; /* used in crt0.S */
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .; /* used in crt0.S */
  } >RAM
}
