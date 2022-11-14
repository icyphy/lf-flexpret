#include <stdint.h>
#include <flexpret_io.h>



#define LEN 20
int lock;
int sema;
int array[LEN];


int main() {
    for (int a = 0; a < LEN; a++) {
        array[a] = 0;
    }
    lock = 0;
    sema = 0;
    _fp_print(123456789);
}

int mt_main(uint32_t thread_id) {

    for (int a = 0; a < LEN; a++) {
        lock_acquire(&lock, thread_id);
        array[a] += 100;
        for (int x = 0; x < 2; x++) {
            int temp = array[a];
        }
        _fp_print(thread_id);
        lock_release(&lock, thread_id);
    }

    sema_up(&sema, thread_id);


    if (thread_id == 0) {
        sema_down(&sema, thread_id);
        sema_down(&sema, thread_id);
        sema_down(&sema, thread_id);
        _fp_print(123456789);
        for (int a = 0; a < LEN; a++) {
            _fp_print(array[a]);
        }
    }
    
}

