#include "thread.h"
#include "lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
lock_t spinlock;
int check_lock = 0;
int func1_lock = 0;
int func2_lock = 0;
int func3_lock = 0;
int var = 0;
int f1(int arg)
{
    printf("Inside function 1 %d\n", arg);

    thread_lock(&spinlock);
    check_lock++;
    thread_unlock(&spinlock);
    func1_lock++;
    return 1;
}
int f2(int arg)
{
    printf("Inside function 2 %d\n", arg);
    thread_lock(&spinlock);
    check_lock++;
    thread_unlock(&spinlock);
    func2_lock++;
    return 2;
}
int f3(int arg)
{
    printf("Inside function 3 %d\n", arg);
    thread_lock(&spinlock);
    check_lock++;
    thread_unlock(&spinlock);
    func3_lock++;
    return 3;
}
int main()
{
    spinlock_init(&spinlock);
    init();
    printf("start of the code\n");
    myth_create(f1, 123);
    printf("middle of the code\n");
    myth_create(f2, 456);
    printf("end of the code\n");
    myth_create(f3, 789);
    myth_join(0);
    myth_join(1);
    myth_join(2);
    printf("%d", check_lock);
    if (check_lock == (func1_lock + func2_lock + func3_lock))
    {
        printf("------------the testing of the lock is successful----------------------------\n");
    }
    else
    {
        printf("----------------the testing of the lock is unsuccessful-----------------------\n");
    }
    free_memory();
}