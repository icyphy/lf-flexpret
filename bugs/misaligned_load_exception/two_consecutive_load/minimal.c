#include <stdint.h>
#include "flexpret_io.h"

#define __print(val) ({ \
    write_csr(CSR_TOHOST, 0xbaaabaaa); \
    write_csr(CSR_TOHOST, val); \
})

uint32_t add(uint32_t a, uint32_t b) {
    return a + b;
}

int main() {
    
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

