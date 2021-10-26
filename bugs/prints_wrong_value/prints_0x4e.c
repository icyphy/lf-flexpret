#include <stdint.h>
#include <flexpret_io.h>

int main() {
    
    uint32_t x = 1;
    _fp_print(x);
    uint32_t y = 2;
    _fp_print(y); // Prints strange number 0x4E

    // Terminate the simulation.
    // Put a while loop to make sure no unwanted side effects.
    _fp_finish();
    while(1) {}
    // Not strictly required; just wanted to let the compiler know.
    __builtin_unreachable();
}

