/*************
Copyright (c) 2021, The University of California at Berkeley.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************/

/** Support file for Bare-metal FlexPRET platform.
 *  
 *  @author{Shaokai Lin <shaokai@berkeley.edu>}
 */

#include <stdio.h>
#include <stdlib.h>
#include "lf_embedded_flexpret_support.h"
#include "core/platform.h"
#include "tinyalloc.h"

#define BILLION    1000000000LL
#define CLOCK_FREQ 10000000LL //FIXME: Check the hardware implementation
// FIXME: Implement the function below.
/**
 * Fetch the value of an internal (and platform-specific) physical clock and 
 * store it in `t`.
 * 
 * Ideally, the underlying platform clock should be monotonic. However, the
 * core lib tries to enforce monotonicity at higher level APIs (see tag.h).
 * 
 * @return 0 for success, or -1 for failure
 */
int lf_clock_gettime(instant_t* t) {
    /*
	uint32_t cycle_high;
	uint32_t cycle_low;
	asm(
	"read_cycle: \n"
		"rdcycleh t0\n"
		"rdcycle %1\n"
		"rdcycleh %0\n"
		"bne t0 %0 read_cycle"
	: "=r"(cycle_high), "=r"(cycle_low) //outputs
	:// inputs
	: "t0" //clobbers
	);

    uint64_t total_cycles = (uint64_t) cycle_high << 32 | cycle_low;
    *t = (instant_t) (total_cycles * BILLION / CLOCK_FREQ);
	return 0;
    */
}

/**
 * Pause execution for a number of nanoseconds.
 *
 * @return 0 for success, or -1 for failure. In case of failure, errno will be
 *  set appropriately (see `man 2 clock_nanosleep`).
 */
int lf_nanosleep(instant_t requested_time) {
    /*
	instant_t t;
	lf_clock_gettime(&t);
    instant_t expire_time = t + requested_time;
	while (t < expire_time) {
		lf_clock_gettime(&t);
	}
	return 0;
    */
}

void lf_initialize_clock() {
}

/**
 * Allocate a requested memory and return a pointer to it.
 */
void *malloc(size_t size) {
	return ta_alloc(size);
}

/**
 * Deallocates the memory previously allocated by a call to calloc, malloc, or realloc.
 */
void free(void *ptr) {
	ta_free(ptr);
}

// Overwrite print functions with NoOp.
int printf(const char *format, ...) {}
int puts(const char *str) {}
int sprintf(char *str, const char *format, ...) {}
int snprintf(char *str, size_t size, const char *format, ...) {}
int vprintf(const char *format, va_list ap) {}
int vfprintf(FILE *stream, const char *format, va_list arg) {}
