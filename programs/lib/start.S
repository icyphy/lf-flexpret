.section .text
.globl _start
.type _start,@function

.macro setStackPointer 
    li sp, 0x2003E800 // 0x3E800 = 256K
.endm

_start:
    setStackPointer	

    // Set the global pointer
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop

    // set all 8 memory regions to shared (rather than exclusive to thread 0)
    li a0, 0x88888888
    csrw 0x50c, a0

    // get thread id
    csrr a0, 0x50b
    
    // give each thread 16KB of stack
    li sp, 0x20040000 // 0x40000 = 256KiB
    sll t0, a0, 14 // (thread_id) * 2^14
    sub sp, sp, t0


    continue:

    // jump to mt_thread_start
    call mt_thread_start
