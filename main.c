#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include <signal.h>
#include <setjmp.h>
// #include <stack.h>
#define running 1
#define waiting 0
int thread_id=2;
int main_thread_id=1;
struct thread {
	int tid;
	void *stack;
    int (*fn) (void *);
	void *arg;
    struct thread *next;
    ucontext_t context;
    int state;
};
struct thread *head=NULL;
struct thread * last=NULL;
void insert(int thread_id, int (*fn) (void *), void *arg,ucontext_t context1){
	if(head==NULL){
		head=(struct thread *)malloc(sizeof(struct thread));
		head->next=NULL;
		head->tid=thread_id;
		head->fn=fn;
        head->stack=malloc(5000);
		head->arg=arg;
        head->context=context1;
        head->state=waiting;
		last=head;
	}
	else{
		struct thread * p=(struct thread *)malloc(sizeof(struct thread));
		p->next=NULL;
		last->next=p;
		last=p;
		p->tid=thread_id;
        p->stack=malloc(5000);
		p->fn=fn;
        p->context=context1;
        head->state=waiting;
		p->arg=arg;
	}
}
void f1(){
    printf("we are in f1\n");
    return 0;
}
void f2(){
    pritf("we are in f2\n");
}
ucontext_t main_context;
void mythread_create(int *tid, int (*fn) (void *), void *arg) {
    ucontext_t context;
    char stack[5000];
    context.uc_stack.ss_sp=stack;
    context.uc_stack.ss_size=sizeof(stack);
    context.uc_link=&main_context;
    makecontext(&context,fn,arg);
    insert(thread_id,fn,0,context);
    thread_id++;
}
void scheduler(){
    // get context of the  given user thread
    // jmp to the main_context
    while(1){
        struct thread p=head;
        if(head->state==waiting){
            head->state=running;
            ucontext_t threadcontext;
            getcontext(&threadcontext);
            // swapcontext()
            // save this context
            // alarm
        }
    }
    // going back to again to main code;
    siglongjmp(env, 1);
}
void signalhandler(){
    // jump to schedulaer;
    printf("we are in the signalhandlr function");
    scheduler();
}
jmp_buf env;
int main(){
    signal(SIGALRM,signalhandler);
    printf("We are in main of the code");
    mythread_create(thread_id,f1,0);
    alarm(5);
    if (sigsetjmp(env, 1) == 0) {
        printf("saving the location of the main to come back again to this position\n");
    }
    // init()
    // main cha context ani schedular cha context manually save karaycha;
}

