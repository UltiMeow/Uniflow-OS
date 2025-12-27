.section .text
.global _start
_start:
    ldr x0, =stack_top
    mov sp, x0          // Initialize stack pointer
    msr sctlr_el1, xzr // Disable MMU for simplicity 
    bl kernel_main
hang:
    wfe
    b hang

.section .bss
.align 4
stack_bottom:
    .space 16384  // 16KB stack
stack_top: