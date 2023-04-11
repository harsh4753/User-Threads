#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#define STACK_SIZE 5000
int thread_id = 0;
typedef struct thread{
    int tid;
    ucontext_t context;
    struct thread* next;
} Node;
void f1(int arg) {
    printf("Inside function 1  %d\n", arg);
}
void f2(int arg) {
    printf("Inside function 2  %d\n", arg);
}
void f3(int arg) {
    printf("Inside function 3  %d\n", arg);
}
Node* head = NULL;
Node* tail = NULL;
ucontext_t main_context;
void myth_create(void (*function)(int), int arg) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->tid = thread_id;
    thread_id++;
    getcontext(&node->context);
    node->context.uc_link = &main_context;
    node->context.uc_stack.ss_sp = malloc(STACK_SIZE);
    node->context.uc_stack.ss_size = STACK_SIZE;
    node->context.uc_stack.ss_flags = 0;
    node->context.uc_link = &main_context;
    makecontext(&node->context, function, 1, arg);
    //insert thread 
    if (tail == NULL) {
        head = node;
        tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    swapcontext(&main_context, &node->context);
}
int main() {
    // main cha context
    getcontext(&main_context);
    printf("start of the code\n");
    myth_create(f1, 123);
    printf("hbebfyerbfyer\n");
    myth_create(f2, 456);
    printf("dijfnuhfuebr\n");
    myth_create(f3, 789);
    printf("huebfyeryfery\n");
    printf("at the end\n");
    // memory free
    for (Node* node = head; node != NULL; ) {
        Node* next_node = node->next;
        free(node->context.uc_stack.ss_sp);
        free(node);
        node = next_node;
    }
    return 0;
}