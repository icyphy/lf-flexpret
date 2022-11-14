#include <unistd.h>      // Declares _exit() with definition in syscalls.c
#include <stdint.h>
#include <flexpret_io.h>

static inline void delay_until(unsigned int high, unsigned int low)
{
  __asm__ __volatile__("addi  x3,%[h],0;"
                       "addi  x2,%[l],0;"
                       ".word 0x00C4015B;"
                       : 
                       : [h]"r"(high), [l]"r"(low)
                       : "x3", "x2");
}

/*
        ____ ____ ____ ____ ____ ____ ...
        t0   t1   t2   t3   t0   t1
*/

/* waits until this thread's "window of access" */
#define WINDOW_LENGTH 16
void wait_for_access(uint32_t thread_id) {
    uint32_t where = thread_id * WINDOW_LENGTH; // where in period
    uint32_t period_mask = 4 * WINDOW_LENGTH - 1; // period - 1
    // assuming up to 4 threads, WINDOW_LENGTH cycles of access each
    // thread has to wait (where - cycle) % period
    // period must be power of 2 so period mask can work

    __asm__ __volatile__(
        "rdcycle t0;"
        "sub t0, %[w], t0;" // (where - cycle)
        "and t0, t0, %[p];" // t0 = (where - cycle) % 64
        "loop_label%=:"
        "addi t0, t0, -3;" // the loop is 3 cycles long: 1 for addi, 2 for bge 
        "bgez t0, loop_label%=;"
        :
        : [w]"r"(where), [p]"r"(period_mask)
        : "t0"
    );
}


void cycle_test() {
    uint32_t c1;
    uint32_t c2;
    uint32_t c3;
    __asm__ __volatile__(
        "rdcycle %[a];"
        "rdcycle %[b];"
        "rdcycle %[c];"        
        : [a]"=r"(c1), [b]"=r"(c2), [c]"=r"(c3)
        :
        :
    );
    _fp_print(c1);
    _fp_print(c2);
    _fp_print(c3);
}

void lock_acquire(uint32_t* lock, uint32_t thread_id) {

    /*
    __asm__ __volatile__(
        "loop_label%=:"
        "mv a0, %[tid];"
        "call wait_for_access;"
        //"lw t1, 0(%[lock]);"
        // "li t0, 1;"
        //"sw t0, 0(%[lock]);"
        //"bnez t1, loop_label%=;"
        :
        : [lock]"r"(lock), [tid]"r"(thread_id)
        : "t0", "t1"
    );
    */


    int status = 1;
    while (status) {
        wait_for_access(thread_id);
        status = *lock;
        *lock = 1;
    }
}

void lock_release(uint32_t* lock, uint32_t thread_id) {
    wait_for_access(thread_id);
    *lock = 0;
}

// lw t0, 0(%[sema]);
// snez t1, t0;
// sub t1, t0, t1;
// sw t1, 0(%[sema]);
// bez t0, loop_label%=;
void sema_down(uint32_t* sema, uint32_t thread_id) {
    uint32_t status = 0;
    while (!status) {
        wait_for_access(thread_id);
        status = *sema;
        if (status) {
            *sema = status - 1;
        }
    }
}

void sema_up(uint32_t* sema, uint32_t thread_id) {
    wait_for_access(thread_id);
    *sema = *sema + 1;
}




volatile uint32_t finished_setup = 0;

void mt_thread_start(uint32_t thread_id) {
    uint32_t f = 0;
    
    if (thread_id == 0) {
        Reset_Handler(thread_id);
        wait_for_access(thread_id);
        finished_setup = 1;
        f = 1;
    }

    while (!f) {
        wait_for_access(thread_id);
        f = finished_setup;
    }
    
    mt_startup(thread_id);

    if (thread_id == 0) {
        _exit(0);
    } else {
        while (1);
    }
}
