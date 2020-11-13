In implementing phase 2, we declared a struct called uthread_tcb which contained
all the necessary information about the thread's context. We did not need to
store the state of the thread in the struct because if a struct was blocked,
then we did not have it in our main thread queue.

uthread_start creates two queues: a main queue and an exited thread queue. It
also creates a main thread and an idle thread which serves the purpose of 
freeing exited threads as well as switching back to our original context once 
all other threads have finished execution.

After the context switch back to our original context, we free up the memory of
the queue. 


Implementing phase 4 was quite straight forward. In preempt_start we create a 
signal handler that yields the current thread and a timer that goes off 100 
times a second. In our first implementation, we had a global variable for
whether preemption was enabled, that would tell the current thread whether it 
should yield or not. We decided that it would instead be better to set up signal
blockers to reduce overhead and minimize use of global variables.

We disable preemption when we access the resource count of our semaphores,
when we access global queues, or when we perform context switches. It was
difficult to figure out how to reenable preemption after a context switch, but
when we realized that a thread would only begin execution when it is 
bootstrapped or after its call to switch contexts, we knew where we needed to 
put the calls.


In order to test preemption, we wrote a program that had two threads. Thread one
creates thread two and then starts a non-yielding while loop that does not 
terminate until a global variable is set to 1. Thread two iterates 5 times and 
yields back to thread one during each iteration. In order for this function to 
terminate, thread 2 must be scheduled, and in order for thread 2 to be scheduled 
thread 1 must be preempted because it never yields.


Testing was done using print statements, gdb and valgrind.
