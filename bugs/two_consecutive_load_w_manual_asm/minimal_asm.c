#include <stdint.h>
#include "flexpret_io.h"

#define __print(val) ({ \
    write_csr(CSR_TOHOST, 0xbaaabaaa); \
    write_csr(CSR_TOHOST, val); \
})

uint32_t add(uint32_t a, uint32_t b) {
    // uint32_t _a = a;
    // uint32_t _b = b;
    uint32_t c;
    asm volatile (
        "lw	t0,%[a]\n\t"
        "lw	t1,%[b]\n\t"
        "add %[c],t0,t1\n\t"
        : [c] "=r" (c)
        : [a] "rm" (a), [b] "rm" (b)
        : "t0", "t1"
       );
    return c;
}

int main() {
    
    // Load 2 values to a5 separately.
    // And call csrw to output them.
    uint32_t x = 1;
    __print(x);

    uint32_t y = 2;
    __print(y);
    
    uint32_t z = add(x, y);
    __print(z);
    
    // Terminate the simulation.
    // Put a while loop to make sure no unwanted side effects.
    _fp_finish();
    while(1) {}
    // Not strictly required; just wanted to let the compiler know.
    __builtin_unreachable();
}

