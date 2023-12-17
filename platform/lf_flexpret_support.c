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
#include <stdarg.h>
#include "../platform.h"

/**
 * Used to keep track of the number of nested critical sections. 
 * 
 * We should only disable interrupts when this is zero and we enter a critical section
 * We should only  enable interrupts when we exit a critical section and this is zero
 */
static int critical_section_num_nested = 0;

/**
 * @return 0 for success, or -1 for failure
 */

static volatile uint32_t last_time = 0;
static volatile uint64_t epoch = 0;
#define EPOCH_DURATION_NS (1ULL << 32)
int lf_clock_gettime(instant_t* t) {
    uint32_t now = rdtime();
    if (now < last_time) {
        epoch += EPOCH_DURATION_NS;
    }
    *t = now + epoch;
    last_time = now;
    return 0;
}

/**
 * @brief Sleep until the given wakeup time.
 * 
 * @param wakeup_time The time instant at which to wake up.
 * @return int 0 if sleep completed, or -1 if it was interrupted.
 */
int lf_sleep_until_locked(instant_t wakeup_time) {

    // FIXME: Handle sleep durations exceeding 32bit nanoseconds range
    // FIXME: Handle async events
    fp_delay_until(wakeup_time);
    return 0;
}

int lf_sleep(interval_t sleep_duration) {
    // FIXME: Handle sleep durations exceeding 32bit
    fp_delay_for(sleep_duration);
}
/**
 * Initialize the LF clock.
 */
void lf_initialize_clock() {}

// In the threaded case, LF provides these itself
#if defined(LF_UNTHREADED)
int lf_critical_section_enter() {
    // In the special case where this function is called during an interrupt 
    // subroutine (isr) it should have no effect
    if ((read_csr(CSR_STATUS) & 0x04) == 0x04) return 0;

    fp_assert(critical_section_num_nested >= 0, "Number of nested critical sections less than zero.");
    if (critical_section_num_nested++ == 0) {
        DISABLE_INTERRUPTS();
    }
    return 0;
}

int lf_critical_section_exit() {
    // In the special case where this function is called during an interrupt 
    // subroutine (isr) it should have no effect
    if ((read_csr(CSR_STATUS) & 0x04) == 0x04) return 0;

    if (--critical_section_num_nested == 0) {
        ENABLE_INTERRUPTS();
    }
    fp_assert(critical_section_num_nested >= 0, "Number of nested critical sections less than zero.");
    return 0;
}

int lf_notify_of_event() {
    return 0;
}
#endif // defined(LF_UNTHREADED)

/**
 * Pause execution for a number of nanoseconds.
 *
 * @return 0 for success, or -1 for failure. In case of failure, errno will be
 *  set appropriately (see `man 2 clock_nanosleep`).
 */
int lf_nanosleep(interval_t requested_time) {
    instant_t t;
    lf_clock_gettime(&t);
    instant_t expire_time = t + requested_time;
    while (t < expire_time) {
        lf_clock_gettime(&t);
    }
    return 0;
}

// Threaded implementation
#if defined LF_THREADED || defined _LF_TRACE

int lf_available_cores() {
    return NUM_THREADS; // Return the number of Flexpret HW threads
}

int lf_thread_create(lf_thread_t* thread, void *(*lf_thread) (void *), void* arguments) {
    // TODO: Decide between HRTT or SRTT
    return fp_thread_create(HRTT, thread, lf_thread, arguments);
}


int lf_thread_join(lf_thread_t thread, void** thread_return) {
    return fp_thread_join(thread, thread_return);
}

int lf_mutex_init(lf_mutex_t* mutex) {
    *mutex = (lf_mutex_t) FP_LOCK_INITIALIZER;
}

int lf_mutex_lock(lf_mutex_t* mutex) {
    fp_lock_acquire(mutex);
    return 0;
}

int lf_mutex_unlock(lf_mutex_t* mutex) {
    fp_lock_release(mutex);
    return 0;
}

int lf_cond_init(lf_cond_t* cond, lf_mutex_t* mutex) {
    *cond = (lf_cond_t) FP_COND_INITIALIZER(mutex);
}

int lf_cond_broadcast(lf_cond_t* cond) {
    return fp_cond_broadcast(cond);
}

int lf_cond_signal(lf_cond_t* cond) {
    return fp_cond_signal(cond);
}

int lf_cond_wait(lf_cond_t* cond) {
    return fp_cond_wait(cond);
}

int lf_cond_timedwait(lf_cond_t* cond, instant_t absolute_time_ns) {
    return fp_cond_timed_wait(cond, absolute_time_ns);
}

// https://www.ibm.com/docs/en/xl-c-and-cpp-aix/16.1?topic=functions-sync-fetch-add
uint32_t __sync_fetch_and_add_4(volatile void *ptr, const uint32_t value) {
    static lf_mutex_t sync_fetch_and_add_lock = FP_LOCK_INITIALIZER;
    lf_mutex_lock(&sync_fetch_and_add_lock);
    const uint32_t prev_value = (*(uint32_t *)ptr);
    (*(uint32_t *) ptr) += value;
    lf_mutex_unlock(&sync_fetch_and_add_lock);
    return prev_value;
}

// https://www.ibm.com/docs/en/xcfbg/121.141?topic=functions-sync-add-fetch
uint32_t __sync_add_and_fetch_4(volatile void *ptr, const uint32_t value) {
    static lf_mutex_t sync_add_and_fetch_lock = FP_LOCK_INITIALIZER;
    lf_mutex_lock(&sync_add_and_fetch_lock);
    const uint32_t new_value = *((uint32_t *) ptr) + value;
    (*(uint32_t *) ptr) = new_value;
    lf_mutex_unlock(&sync_add_and_fetch_lock);
    return new_value;
}

// https://www.ibm.com/docs/en/xl-c-and-cpp-aix/16.1?topic=functions-sync-bool-compare-swap
bool __sync_bool_compare_and_swap_4(volatile void *ptr, uint32_t compare_value, uint32_t new_value) {
    static lf_mutex_t sync_bool_compare_and_swap_lock = FP_LOCK_INITIALIZER;
    lf_mutex_lock(&sync_bool_compare_and_swap_lock);
    const bool retval = (*(bool *) ptr) == compare_value;
    
    // Swap only occurs if condition is true
    if (retval) {
        (*(bool *) ptr) = new_value;
    }
    lf_mutex_unlock(&sync_bool_compare_and_swap_lock);
    return retval;
}

#endif

/**
 * We are not using vfprintf nor svfprintf and these functions have huge
 * implementations. Doing this saves space. 
 * 
 */
int _vfprintf_r(struct _reent *reent, FILE *fp, const char *fmt, va_list list) {}
int _svfprintf_r(struct _reent *reent, FILE *fp, const char *fmt, va_list list) {}
