#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "thread.h"

void thread_exit(void *retval) {
  _exit(0);
}

thread_t thread_self() {
  return getpid();
}

thread_t thread_create(thread_t *thread, int (*fn) (void *), void *arg) {
    int tid;
    char *stack = malloc(4096);
    tid = clone((int (*)(void *))fn, stack + 4096, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND, arg);
    *thread = tid;
    printf("thread id = %d\n", *thread);
    return tid;
}


int thread_join(thread_t thread) {
  int status;
  if (waitpid(thread, &status, 0) == -1) {
    perror("waitpid");
    return -1;
  }
  return status;
}

