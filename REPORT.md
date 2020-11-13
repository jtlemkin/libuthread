# Project Two
##### Nicholas Treynor and James Lemkin

### Queue Implementation

The basis of our queue structure is that of a linked list, with the queue struct
itself containing pointers to a head and tail node, as well as an integer 
"Length" containing the queue's length and another integer operating as a 
boolean value to inform us if a deletion has occurred recently. Each node 
contained within the queue possesses a void pointer that we store data in, and a
pointer to the next queue_node struct in the queue, initialized to NULL on 
creation and updated appropriately when a node is appended to the queue's tail. 
(Or in the base case, initialized as the queue's head)

The reason we store the length as an integer, and update it accordingly when the
length of the queue changes is so that we can determine the length of a queue in
O(1) time, instead of attempting to iterate over the queue and count the length.

One benefit of using a linked list implementation as we did is that it is very 
simple to enqueue and dequeue objects from the queue, consisting of some basic 
pointer manipulation, just being certain that we handle cases where we insert
the first node when enqueueing data, and dequeue the last node in the queue, 
appropriately. 

An alternative method for implementing the queue could have been a dynamically 
resizing array, but we felt that it would be easier to implement it as a linked 
list, as managing a resizing array would have been more complicated to manage 
and would likely have run slower. As a result of the increased complexity 
inherent to resizing arrays, it would also likely have been more prone to 
breakage. 

#### Queue Iterate & Queue Delete: 

The queue_iterate and queue_delete methods are a bit outside of the scope of a 
typical queue implementation, and so deserve a bit more attention. In our delete
method, we start from the head of the queue and move through the linked list 
using a while loop, updating "curr_node" and "prior_node" to point to the node 
whose value we are currently checking by using the inherent connections between 
the nodes of the queue established in the linked list structure, and the node 
examined previously. Upon finding the node whose data we wish to delete from the 
queue, we check the current state of the prior node to determine if we are at 
the head, tail, or middle of the node, and overwrite pointers of the appropriate
nodes and the queue in order to perform the deletion appropriately.

Our iteration method moves along the queue in a similar manner, calling func()
on every bit of data in the queue. This is where the int we store in the queue 
to inform us if a deletion occurs comes into play. As we could perform a 
deletion while iterating over the queue, we cannot always trust that the 
pointers we access will be valid. We use the "deletionOccurred" int to inform us 
if a change has just been made by the call to func(), (updated in queue_delete)
allowing us to react appropriately and move to the node that would
have followed the node we were operataing on, had it not been removed from the 
queue. 

#### Limitations:

Though our queue_iterate function is resistant to deletions of the data that is 
currently being operated on by the function passed to queue_iterate, we suspect 
it would break down if someone were to pass a function that performed multiple 
deletions from operation on the same node, as if someone were to delete the next
node in the queue when operating on a given node, we would lose the pointer to 
the appropriate next node. We could handle this by creating a copy of the queue 
to utilize in such cases, but we were informed by a piazza post that this would 
be too much overhead for the queue_iterate() function, and that we would only 
need to handle cases where deletion occurs on the node currently being operated 
on. To that end, we do handle deletion at the head, middle, and tail of the 
queue appropriately. 

### Uthread Implementation

In implementing phase 2, we declared a struct called uthread_tcb which contained
all the necessary information about the thread's context. We did not need to 
store the state of the thread in the struct because if a struct was blocked, 
then we did not have it in our main thread queue. We did however store a 
uthread_ctx_t object to contain the thread's context, as well as a pointer to 
the stack to be used by the TCB.

uthread_start creates two queues: a main queue and an exited thread queue. It 
also creates a main thread, in addition to an idle thread which serves the 
purpose of freeing exited threads, as well as switching back to our original 
context once all other threads have finished execution.

After creating the above threads, we kick off execution of the first thread by 
removing it from the queue they are added to when uthread_create() is called, 
and performing a context switch to it. From this point on, we cycle through the 
different threads as they call yield, moving threads from the queue into 
execution as we store prior threads back onto the queue, until threads complete 
and are removed. 

After the context switch back to our original context, we free up the memory of 
the queues we created, to ensure we have no memory leaks in our code.

### Semaphore Implementation

Our semaphores are implemented in a manner slightly different than 
demonstrated during lecture in order to avoid starvation. A semaphore struct is 
constructed by a call to sem_create(), and creates a semaphore with a new queue
used to store blocked threads, and a count indicating the number of resources
the semaphore is capable of providing, as specified by the parameter "count".
sem_down() checks if enough resources are available, and if so decrements the 
count of resources available. If there are not enough resources available to 
accommodate a thread's request, the thread is added to the semaphore's queue of 
blocked threads, while simultaneously being removed from the main queue of 
executing threads by uthread_block();

The magic in our code occurs partially here, where we do not perform a while 
loop over uthread_block(), and partially in sem_up(), where we more carefully 
restrict which threads have access to the given semaphore, in order to avoid 
starvation. 

When a call to sem_up() is made, the semaphore's count is incremented and a 
thread is released from the queue of blocked threads if there is one present, to
be added back to the main queue via a call to uthread_unblock. The crucial part 
of our implementation is this: If a thread is released from the queue of blocked
threads, we immediately decrement the count once more. This prevents other 
threads from snatching the resource before the unblocked thread can gain access 
to it, and places other queues that would have done so into a blocked state, in 
order to wait their turn for the resource, to be released when another thread 
calls sem_up(). 

In this manner, we maintain the functionality of a semaphore, but avoid 
starvation using a rather elegant methodology.

### Preemption Implementation

Implementing phase 4 was quite straight forward. In preempt_start we create a 
signal handler that yields the current thread and a timer that goes off 100 
times a second. In our first implementation, we had a global variable for 
whether preemption was enabled, that would tell the current thread whether it 
should yield or not. We decided that it would instead be better to set up signal
blockers to reduce overhead and minimize use of global variables.

We disable preemption when we access the resource count of our semaphores, when 
we access global queues, or when we perform context switches. It was difficult 
to figure out how to reenable preemption after a context switch, but when we 
realized that a thread would only begin execution when it is bootstrapped or 
after its call to switch contexts, we knew where we needed to put the calls.

In order to test preemption, we wrote a program that had two threads. Thread one
creates thread two and then starts a non-yielding while loop that does not 
terminate until a global variable is set to 1. Thread two iterates 5 times and 
yields back to thread one during each iteration. In order for this function to 
terminate, thread 2 must be scheduled, and in order for thread 2 to be scheduled
thread 1 must be preempted because it never yields.

### Testing

Testing was done using print statements, gdb and valgrind. We extensively tested
our code using the provided test cases, modified versions of the provided test 
cases that increased complexity, and test cases we developed on our own, such as
"preempt_two_threads.c", which we created to test our preemption. We also went 
back and ensured our code was able to run on test cases for prior phases post 
implementation of preemption.

### Sources

We heavily consulted lecture notes and piazza to create our library, but made
very minimal use of outside sources to complete our project aside from the GNU 
man pages on signals and signal blocking.