# user-threads


## Overview:
threadLib is a multithreading library with three types of mapping models.</br>
**1. One-One**</br>
**2. Many-One**</br>
**3. Many-Many**</br>
threadLib can be used to achieve parllel flow of execution of user program. It is available for Unix-like POSIX operating systems.


## 1. One-One Model:
    Each user thread maps to one kernel thread. Kernel thread is created with clone system call.
    Newly created thread is inserted in the linked list of threads. 
    Each node of the linked list contains the metadata of corresponding thread.

<p align="center">
  <img 
    width="450"
    height="450"
    src="https://user-images.githubusercontent.com/61696982/167251262-0fa23612-5db8-4e1a-9fea-f6c9c21444a4.png"
  >
</p>


## 2. Many-One Model:
    All the user threads created by user will run on only one kernel thread.
    Scheduler is use to schedule the many user threads on one kernel thread. 
    Scheduler has its own context. Here main is also treated like other threads.

<p align="center">
  <img 
    align="center"
    width="450"
    height="450"
    src="https://user-images.githubusercontent.com/61696982/167251431-e1ead0ea-c334-43fa-8823-59a7de440e68.png"
  >
</p>

### Flow of execution:
        1. Suppose thread T1 is executing, after completing its time quantum it would receive an Timer Interrupt.
        2. User defined signal handler of corresponding thread T1 will be invoked.
        3. In signal handler the context of current thread is saved and we jump to Scheduler context.
        4. Scheduler finds a RUNNABLE process and jumps to its context.
        5. If newly selected thread has executed before for some time then the thread would resume execution in the 
           signal handler where context was stored previously in step 3. 
           Otherwise thread will start execution in wrapper function which is an argument to clone.
        6. This thread will execute for its time quantum and the same process will be repeated.

        Wrapper Function(execute_me in code):
                Helps in identifying when thread is over. 
                It simply calls the user function corresponding to thread.

<p align="center">
  <img 
    align="center"
    src="https://user-images.githubusercontent.com/61696982/167254634-64a9ec32-aee4-4fb6-872f-791a25db91a9.png"
  >
</p>





