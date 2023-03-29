typedef int thread_t;
int thread_create(thread_t *thread, int (*fn) (void *), void *arg);
int thread_join(thread_t thread);
void thread_exit(void *retval);
thread_t thread_self();

