#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <sys/time.h>
#include "thread.h"
int thread_id = 0;
int return_array[100];
jmp_buf env;
ucontext_t main_context;
Node *main_queue = NULL;
Node *main_queue_tail = NULL;
Node *queue_head=NULL;
Node *queue_last=NULL;
Node *current_thread = NULL;
void wrapper(int (*function)(void *), void *xyz, int th_id)
{
    int a = function(xyz);
    printf("function runned successfully and return value is  %d\n", a);
    return_array[th_id] = a;
    return ;
}
void signalhandler(int xyz)
{
    printf("we are in signal handler\n");
    scheduler();
}
void insertNodeAtEnd(Node *newNode)
{
    if (queue_head == NULL)
    {
        queue_head = newNode;
        queue_last = newNode;
        queue_head->prev = queue_last;
        queue_last->next = queue_head;
    }
    else
    {
        queue_last->next =newNode;
        newNode->prev = queue_last;
        newNode->next = queue_head;
        queue_head->prev = newNode;
        queue_last = newNode;
       
    }
    printf("node insertion successfully\n");
}
void deleteNode(Node* head, int threadid) {
    printf(" deleting the node\n");
    Node* current = head;
    while (current != NULL && current->tid !=threadid ) {
        
        current = current->next;
        if(current==head){
            return ;
        }
    }
    if (current == NULL) {
        return;
    }
    if (current->prev != NULL) {
        current->prev->next = current->next;
    } else {
        head = current->next;
    }
    if (current->next != NULL) {
        current->next->prev = current->prev;
    }
    printf("deleted node successfully\n");
    free( current);
    return;
}

void scheduler()
{
    printf("we are in scheduler\n");
    if (current_thread != NULL)
    {
        current_thread->status = waiting;
        // save current thread's context
        swapcontext(&current_thread->context, &main_context);
    }
    Node *p = main_queue;
    // printf("below");
    while (p)
    {
        // printf("hello");
        if (p->status == waiting)
        {
            p->status = running;
            p->timer.it_interval.tv_sec = 0;
            p->timer.it_interval.tv_usec = 0;
            p->timer.it_value.tv_sec = TIME_QUANTUM;
            p->timer.it_value.tv_usec = 0;
            setitimer(ITIMER_REAL, &p->timer, NULL);
            printf("timer is set to  thread\n");
            current_thread = p;
            swapcontext(&main_context, &p->context);
            current_thread = NULL;
            printf("completing the process\n");
            if (return_array[p->tid])
            {
                printf("the process is completed\n");
                // p->status = waiting;
                p->status = completed; 
                printf("the thread  %d completed its excution",p->tid);
            }
            else
            {
                printf("the process is waiting\n");
                // p->status =completed;
                p->status = waiting;
                int thread_delete_tid = p->tid;
                
                Node *temp=(Node*)malloc(sizeof(Node));
                temp->tid=p->tid;
                temp->context=p->context;
                temp->next=NULL;
                temp->prev=NULL;
                temp->status=waiting;
                insertNodeAtEnd(temp);
                printf("the thread with id %d is inserted in the queue\n",thread_delete_tid);
                p->status = completed;
                // p=NULL;
                // insertNode(p->tid, p->context,2, &queue_head) ;

                deleteNode(main_queue,thread_delete_tid);
                printf("the thread with id %d deleted from the main queue \n",thread_delete_tid);
            }
            // break;
            return;
        }
        p = p->next;
        if (p == main_queue)
        {
            break;
        }
    }
}

void init()
{
    printf("initialising the array from the return value given by the functions\n");
    for (int i = 0; i < 100; i++)
    {
        return_array[i];
    }
}

void myth_create(int (*function)(int), int arg)
{
    if (thread_id == 0)
    {
        signal(SIGALRM, signalhandler);
    }
    Node *node = (Node *)malloc(sizeof(Node));
    node->tid = thread_id;
    thread_id++;
    getcontext(&node->context);
    node->context.uc_link = &main_context;
    node->context.uc_stack.ss_sp = malloc(STACK_SIZE);
    node->context.uc_stack.ss_size = STACK_SIZE;
    node->context.uc_stack.ss_flags = 0;
    node->status = waiting;

    makecontext(&node->context, (void (*)())wrapper, 3, function, arg, node->tid);
    printf("the making of the context of the thread is done\n");
    // insert thread
    if (main_queue_tail == NULL)
    {
        main_queue = node;
        main_queue_tail = node;
        main_queue_tail->next = main_queue;
        main_queue->prev = main_queue_tail;
    }
    else
    {
        node->prev = main_queue_tail;
        main_queue_tail->next = node;
        main_queue_tail = node;
        main_queue_tail->next = main_queue;
        main_queue->prev = main_queue_tail;
    }
    printf("the thread with waiting status is inserted in the main queue\n");
    alarm(2);
    sleep(4);
}

void myth_join(int tid)
{
    sleep(2);
    Node *xy = queue_head;
    while (xy)
    {   
    //  swapcontext(&main_context,&xy->context);
        if (xy->tid == tid)
        {
            while (xy->status != completed|| !return_array[tid])
            {
                // scheduler();
                // setcontext(&main_context);
                printf("earlier runned thread is  ruuning again\n");
                printf("join for thread %d successful\n",tid);
                swapcontext(&main_context,&xy->context);
                xy->status=completed;
                break;
            }
            return;
        }
        xy =xy->next;
        if(xy==queue_head){
            return;
        }
    }
    printf("mythread join successful\n  ");
}

void free_memory()
{
    
    for (Node *node = main_queue; node != NULL;)
    {
        Node *next_node = node->next;
        free(node->context.uc_stack.ss_sp);
        free(node);
        node = next_node;
        if (node == main_queue)
        { 
            break;
        }
    }
    printf("allocated memory free ");
}
void print_queue(){
    if(queue_head){
        printf("the queue is generated\n");
    }
}
void thread_cancel(int tid) {
    Node* thread = main_queue;
    while (thread != NULL) {
        if (thread->tid == tid) {  
            thread->status = completed;
            printf("thread cancel succeesfully ");
            setcontext(&main_context);
            return;
        }
        thread = thread->next;
    }
}
void thread_kill(int tid) {
    Node* thread = main_queue;
    while (thread != NULL) {
        if (thread->tid == tid) {
            if (thread == current_thread) {
                scheduler();
            }
            if (thread->prev != NULL) {
                thread->prev->next = thread->next;
            } else {
                main_queue = thread->next;
            }
            if (thread->next != NULL) {
                thread->next->prev = thread->prev;
            }
            printf("thread kill successful");
            free(thread->context.uc_stack.ss_sp);
            free(thread);
            return;
        }
        thread = thread->next;
    }
}