#include <stdint.h>
#include <flexpret_io.h>

int main() {
    while(1) {
        uint32_t state = gpo_read_0();
        gpo_write_0(~state);
        _fp_print(state);
        for (uint32_t i = 0; i < 50000; i++) {}
    }
}
