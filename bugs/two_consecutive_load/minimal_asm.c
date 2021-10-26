#include <stdint.h>
#include "flexpret_io.h"

#define __print(val) ({ \
    write_csr(CSR_TOHOST, 0xbaaabaaa); \
    write_csr(CSR_TOHOST, val); \
})

/*
uint32_t add(uint32_t a, uint32_t b) {
  74:	fe010113          	addi	sp,sp,-32
  78:	00812e23          	sw	s0,28(sp)
  7c:	02010413          	addi	s0,sp,32
  80:	fea42623          	sw	a0,-20(s0)
  84:	feb42423          	sw	a1,-24(s0)
    return a + b;
  88:	fec42703          	lw	a4,-20(s0)
  8c:	fe842783          	lw	a5,-24(s0)
  90:	00f707b3          	add	a5,a4,a5
}
  94:	00078513          	mv	a0,a5
  98:	01c12403          	lw	s0,28(sp)
  9c:	02010113          	addi	sp,sp,32
  a0:	00008067          	ret
*/
void add(uint32_t a, uint32_t b) {
    asm("addi	sp,sp,-32\n\t"
        "sw	s0,28(sp)\n\t"
        "addi	s0,sp,32\n\t"
        "sw	a0,-20(s0)\n\t"
        "sw	a1,-24(s0)\n\t"
        "lw	a4,-20(s0)\n\t"
        "lw	a5,-24(s0)\n\t"
        "nop"
        "add	a5,a4,a5\n\t"
        "mv	a0,a5\n\t"
        "lw	s0,28(sp)\n\t"
        "addi	sp,sp,32\n\t"
        "ret"
    );
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

