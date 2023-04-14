#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <setjmp.h>
#define running 1
#define waiting 2
#define completed 3
#define STACK_SIZE 5000
int thread_id = 0;
jmp_buf env;
ucontext_t main_context;
typedef struct thread{
    int tid;
    ucontext_t context;
    struct thread* next;
    int status;
} Node;
Node* head = NULL;
Node* tail = NULL;
void f1(int arg) {
    printf("Inside function 1 %d\n", arg);
    // while(1);
}
void f2(int arg) {
    printf("Inside function 2 %d\n", arg);
}
void f3(int arg) {
    printf("Inside function 3 %d\n", arg);
    while(1);
}
void signalhandler(int xyz){
    printf("we are in signal handler\n");
    scheduler();
}
void scheduler(){
    printf("we are in scheduler\n");
    Node *p = head;
    while(p) {
        if(p->status == waiting) {
            p->status = running;
            swapcontext(&main_context, &p->context);
            p->status=completed;
            // if (sigsetjmp(env, 1) == 0) {
            //     swapcontext(&main_context, &p->context);
            // }
            break;
        }
        p = p->next;
    }
}
void myth_create(void (*function)(int), int arg) {
    if(thread_id==0){
        signal(SIGALRM, signalhandler);
    }
    Node* node = (Node*)malloc(sizeof(Node));
    node->tid = thread_id;
    thread_id++;
    getcontext(&node->context);
    node->context.uc_link = &main_context;
    node->context.uc_stack.ss_sp = malloc(STACK_SIZE);
    node->context.uc_stack.ss_size = STACK_SIZE;
    node->context.uc_stack.ss_flags = 0;
    node->context.uc_link = &main_context;
    node->status = waiting;
    makecontext(&node->context, (void (*)())function, 1, arg);
    // insert thread 
    if (tail == NULL) {
        head = node;
        tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    // scheduler();
    alarm(5);
    sleep(6);
}
void myth_join(int tid){
    Node* p = head;
    while(p){
        if(p->tid == tid){
            while(p->status != completed){
                // scheduler();
            }
            return;
        }
        p = p->next;
    }
}
int main() {
    // main cha context
    // getcontext(&main_context) initializing;
    printf("start of the code\n");
    myth_create(f1, 123);
    printf("hbebfyerbfyer\n");
    myth_create(f2, 456);
    printf("dijfnuhfuebr\n");
    myth_create(f3, 789);
    printf("huebfyeryfery\n");
    printf("at the end\n");
    // memory free
    myth_join(0);
    myth_join(1);
    myth_join(2);
    for (Node* node = head; node != NULL; ) {
        printf("%djfnvuvbueyrgyer gyeryd\n",node->tid);
        Node* next_node = node->next;
        free(node->context.uc_stack.ss_sp);
        free(node);
        node = next_node;
    }
    return 0;
}
