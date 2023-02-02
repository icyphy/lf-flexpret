

.global wait_for_access
.global lock_acquire
.global lock_release
.global sema_down
.global sema_up

#define WINDOW_SHIFT 3 
// window length is 1 << WINDOW_SHIFT = 16
#define PERIOD_MASK 31
// period mask is 4 * (window length) - 1 
// assuming 4 threads
#define ADDI_BGEZ_CYCLES -3
// how many cycles an addi and a bgez take

wait_for_access:
    slli t1,a0, WINDOW_SHIFT
    li t2, PERIOD_MASK
    rdcycle	t0
    sub	t0,t1,t0
    and	t0,t0,t2
    wait_for_access_loop:
        addi t0,t0, ADDI_BGEZ_CYCLES
        bgez t0, wait_for_access_loop
    nop
    ret


lock_acquire:
    addi sp,sp,-16 // 16 byte aligned
    sw ra,12(sp)
    sw s0,8(sp)
    sw s1,4(sp)
    mv s0,a0
    mv s1,a1
    lock_acquire_loop:
        mv a0,s1
        jal ra, wait_for_access
        li t1, 1
        lw t0,0(s0)
        sw t1,0(s0)
        bnez t0, lock_acquire_loop
    lw ra,12(sp)
    lw s0,8(sp)
    lw s1,4(sp)
    addi sp,sp,16    
    ret


lock_release:
    addi sp,sp,-16 // 16 byte aligned
    sw ra,12(sp)
    sw s0,8(sp)
    mv s0,a0
    mv a0,a1
    jal wait_for_access
    sw x0, 0(s0)
    lw ra,12(sp)
    lw s0,8(sp)
    addi sp,sp,16
    ret


sema_down:
    addi sp,sp,-16 // 16 byte aligned
    sw ra,12(sp)
    sw s0,8(sp)
    sw s1,4(sp)
    mv s0,a0
    mv s1,a1
    sema_down_loop:
        mv a0,s1
        jal ra, wait_for_access
        lw t0,0(s0)
        sltiu t1, t0, 1
        xori t1, t1, 1
        sub t1, t0, t1
        sw t1,0(s0)
        bnez t0, sema_down_loop
    lw ra,12(sp)
    lw s0,8(sp)
    lw s1,4(sp)
    addi sp,sp,16    
    ret


sema_up:
    addi sp,sp,-16 // 16 byte aligned
    sw ra,12(sp)
    sw s0,8(sp)
    mv s0,a0
    mv a0,a1
    jal wait_for_access
    lw t0, 0(s0)
    addi t0, t0, 1
    sw t0, 0(s0)
    lw ra,12(sp)
    lw s0,8(sp)
    addi sp,sp,16
    ret
