#ifndef MYTH_H
#define MYTH_H
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <sys/time.h>
#define running 1
#define waiting 2
#define completed 3
#define STACK_SIZE 5000
#define TIME_QUANTUM 2 
typedef struct thread {
    int tid;
    ucontext_t context;
    struct thread* next;
    struct thread* prev;
    int status;
    struct itimerval timer;
} Node;
void thread_cancel(int tid);
void insertNodeAtEnd(Node *newNode);
void insertNode(int tid, ucontext_t context, int status, Node** head) ;
// int f1(int arg);
// int f2(int arg);
// int f3(int arg);
void insertNode(int tid, ucontext_t context, int status, Node** head) ;
void signalhandler(int xyz);
void scheduler();
void myth_create(int (*function)(int), int arg);
void myth_join(int tid);
void free_memory();
void init();
void print_queue();
void wrapper(int (*function )(void*),void*  xyz,int th_id);
// void scheduler_join()
void deleteNode(Node* head, int threadid);
#endif
