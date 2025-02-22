.syntax unified

.section .text
    .thumb

    .extern __text_end__
    .extern __data_beg__
    .extern __data_end__
    .extern __bss_beg__
    .extern __bss_end__
    .extern __stack_end__
    .extern main

    .align 2
    .thumb_func
    .globl  __main
    .type   __main, %function
__main:

    // copy initialized data from flash to ram
copy_data:
    ldr     r1, DATA_BEG
    ldr     r2, TEXT_END
    ldr     r3, DATA_END
    subs    r3, r3, r1      // length of initialized data
    beq     zero_bss        // skip if no initialized data

copy_data_loop:
    ldrb    r4, [r2], #1    // read byte from flash
    strb    r4, [r1], #1    // store byte to ram
    subs    r3, r3, #1      // decrement counter
    bgt     copy_data_loop  // repeat until done

    // fill uninitialized data (bss) with zeros
zero_bss:
    ldr     r1, BSS_BEG
    ldr     r3, BSS_END
    subs    r3, r3, r1      // length of uninitialized data
    beq     run             // skip if no uninitialized data

    mov     r2, #0
zero_bss_loop:
    strb    r2, [r1], #1    // store zero
    subs    r3, r3, #1      // decrement counter
    bgt     zero_bss_loop   // repeat until done

run:
    // initialize system components
    bl      __system

    // call user main function
    mov     r0, #0 // argc=0
    mov     r1, #0 // argv=NULL
    bl      main

TEXT_END: .word __text_end__
DATA_BEG: .word __data_beg__
DATA_END: .word __data_end__
BSS_BEG:  .word __bss_beg__
BSS_END:  .word __bss_end__

    .size __main, . - __main

// vi: ft=armv4
