#include <stdlib.h>
#include <stdint.h>
#include <flexpret_io.h>

int main() {

    uint32_t* p = malloc(sizeof(uint32_t));
    *p = 100;
    _fp_print(*p);
    free(p);

    // Terminate the simulation
    _fp_finish();
    while(1) {}
    __builtin_unreachable();
}
