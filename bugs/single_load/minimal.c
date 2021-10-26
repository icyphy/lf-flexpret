#include <stdint.h>
#include "flexpret_io.h"

#define __print(val) ({ \
    write_csr(CSR_TOHOST, 0xbaaabaaa); \
    write_csr(CSR_TOHOST, val); \
})

/*
void __print(uint32_t v) {
    write_csr(CSR_TOHOST, 0xbaaabaaa);
    write_csr(CSR_TOHOST, v);
}
*/

uint32_t add(uint32_t a) {
    return a + 2;
}

int main() {
    
    // Load 2 values to a5 separately.
    // And call csrw to output them.
    uint32_t x = 1;
    __print(x);
    // _fp_print(x);
    // write_csr(CSR_TOHOST, 0xbaaabaaa);
    // write_csr(CSR_TOHOST, x);
    /*
    asm volatile (
        "csrw 0x51e, %0"
        :
        : "r" (x)
    );
    */
    uint32_t y = 2;
    __print(y);
    // _fp_print(y); // Somehow calling _fp_print() twice causes the issue.
    // write_csr(CSR_TOHOST, 0xbaaabaaa);
    // write_csr(CSR_TOHOST, y);
    /*
    asm volatile (
        "csrw 0x51e, %0"
        :
        : "r" (y)
    );
    */
    
    uint32_t z = add(x);
    __print(z);

    /*
    asm volatile ("li a5, 1");
    asm volatile ("csrw 0x51e, a5");
    asm volatile ("li a5, 2");
    asm volatile ("csrw 0x51e, a5");
    asm volatile ("li a5, 3");
    asm volatile ("csrw 0x51e, a5");
    */
    
    // Terminate the simulation.
    // Put a while loop to make sure no unwanted side effects.
    _fp_finish();
    while(1) {}
    // Not strictly required; just wanted to let the compiler know.
    __builtin_unreachable();
}

