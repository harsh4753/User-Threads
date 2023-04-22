#ifndef MYTH_H1
#define MYTH_H1
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdatomic.h>
typedef struct {
    atomic_flag lock;
} lock_t;
void spinlock_init(lock_t *lock);
void thread_lock(lock_t *lock) ;
void thread_unlock(lock_t *lock);
#endif
