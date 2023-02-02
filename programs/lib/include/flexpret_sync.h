#include <stdint.h>

#ifndef FLEXPRET_SYNC_H
#define FLEXPRET_SYNC_H


/*
For locks, 0 means unlocked and 1 means locked
For semaphores, 0 means no resources availible 
*/


void wait_for_access(uint32_t thread_id);
void lock_acquire(uint32_t* lock, uint32_t thread_id);
void lock_release(uint32_t* lock, uint32_t thread_id);
void sema_down(uint32_t* sema, uint32_t thread_id);
void sema_up(uint32_t* sema, uint32_t thread_id);


#endif // SYNC_H
