#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
int f1(int arg)
{
    printf("Inside function 1 with argument %d\n", arg);
    // while(1){
    //     printf("x");
    // }
    return 1;
}
int f2(int arg)
{
    printf("Inside function 2 with argument  %d\n", arg);
    // while(1){
    //     printf("y");
    // }
    return 2;
}

int f3(int arg)
{
    printf("Inside function 3 with argument %d\n", arg);
    // while(1){
    //     printf("z");
    // }
    return 3;
}
int main()
{
    init();
    printf("start of the code\n");
    myth_create(&f1, 123);
    printf("middle of the code\n");
    myth_create(&f2, 456);
    printf("end of the code\n");
    myth_create(&f3, 789);
    printf("join is first thread\n");
    myth_join(0);
    myth_join(1);
    myth_join(2);
    free_memory();
}