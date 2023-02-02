#include <unistd.h>      // Declares _exit() with definition in syscalls.c.
#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_sync.h>
#include "tinyalloc/tinyalloc.h"

#define DSPM_LIMIT          ((void*)0x2003E800) // 0x3E800 = 256K
#define TA_MAX_HEAP_BLOCK   1000
#define TA_ALIGNMENT        4

extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t end;

//prototype of main
int main();
int mt_main(uint32_t thread_id);

/**
 * Allocate a requested memory and return a pointer to it.
 */
void *malloc(size_t size) {
    return ta_alloc(size);
}

/**
 * Allocate a requested memory and return a pointer to it.
 */
void *calloc(size_t nitems, size_t size) {
    return ta_calloc(nitems, size);
}

/**
 * Deallocate the memory previously allocated by a call to calloc, malloc, or realloc.
 */
void free(void *ptr) {
    ta_free(ptr);
}

/**
 * Initialize initialized global variables, set uninitialized global variables
 * to zero, configure tinyalloc, and jump to main.
 */
void Reset_Handler() {
    // Copy .data section into the RAM
    uint32_t size   = &__data_end__ - &__data_start__;
    uint32_t *pDst  = (uint32_t*)&__data_start__;       // RAM
    uint32_t *pSrc  = (uint32_t*)&__etext;              // ROM

    for (uint32_t i = 0; i < size; i++) {
        *pDst++ = *pSrc++;
    }

    // Init. the .bss section to zero in RAM
    size = (uint32_t)&__bss_end__ - (uint32_t)&__bss_start__;
    pDst = (uint32_t*)&__bss_start__;
    for(uint32_t i = 0; i < size; i++) {
        *pDst++ = 0;
    }

    // Initialize tinyalloc.
    ta_init( 
        &end, // start of the heap space
        DSPM_LIMIT,
        TA_MAX_HEAP_BLOCK, 
        16, // split_thresh: 16 bytes (Only used when reusing blocks.)
        TA_ALIGNMENT
    );

    // Call main().
    main();

}


volatile uint32_t finished_setup = 0;

void mt_thread_start(uint32_t thread_id) {
    uint32_t f = 0;
    
    // thread 0 should call Reset_Handler, which
    // does setup and calls main()
    if (thread_id == 0) {
        Reset_Handler(thread_id);
        wait_for_access(thread_id);
        finished_setup = 1;
        f = 1;
    }

    // other threads wait for thread 0 to finish
    while (!f) {
        wait_for_access(thread_id);
        f = finished_setup;
    }
    
    // Call mt_main().
    mt_main(thread_id);

    if (thread_id == 0) {
        _exit(0);
    } else {
        while (1);
    }
}
