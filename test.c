#include <stdio.h>
#include "thread.h"

int x = 48;

void *f(void *arg) {
  int p = *(int *)arg;
  printf("%d x= %d\n", p, x);
  thread_exit(NULL);  
}

int main() {
  thread_t tid[3];
  int args[3], j, k;

  for(k = 0; k < 3; k++) {
    args[k] = 10 + k;
    thread_create(&tid[k], (int (*)(void *))f, &args[k]);

  }
  
  for(k = 0; k < 3; k++) {
    thread_join(tid[k]);
  }

  return 0;
}

