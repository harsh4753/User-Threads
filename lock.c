
#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <sys/time.h>
#include "lock.h"
void spinlock_init(lock_t *lock) {
    lock->lock = (atomic_flag)ATOMIC_FLAG_INIT;
    printf("Initialising spin lock\n");
}
void thread_lock(lock_t *lock) {
    while (atomic_flag_test_and_set(&lock->lock)) {
        // Keep spinning until the lock is acquired
    }
    printf("acquired the spin lock\n");
}

// Release the spinlock
void thread_unlock(lock_t *lock) {
    atomic_flag_clear(&lock->lock);
    printf("released the spin lock\n");
}



