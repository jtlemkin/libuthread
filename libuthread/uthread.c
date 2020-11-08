#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

// General idea. Create a queue of thread objects.

#define STACK_SIZE 32768

typedef struct uthread_tcb* uthread_tcb_t;

uthread_tcb_t* currTCB;

static uthread_ctx_t mainContext[1];
struct queue_t globalQueue;
struct uthread_tcb {
	/* TODO Phase 2 */

	/*
	 * An example of information contained within a TCB is:

Thread Identifier: Unique id (tid) is assigned to every new thread
Stack pointer: Points to thread's stack in the process
Program counter: Points to the current program instruction of the thread
State of the thread (running, ready, waiting, start, done)
Thread's register values
Pointer to the Process control block (PCB) of the process that the thread lives on

	 */

	// A state
	// A backup of registers
	// a stack.
	int state;
	uthread_ctx_t *uctx;
    void *top_of_stack;
};

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2 */
	return currTCB;
}

void uthread_yield(void)
{
	/* TODO Phase 2 */

	// General idea: Dequeue the next thread on the

    struct uthread_tcb* nextThread;
    queue_dequeue(globalQueue, (void**) &nextThread);

    uthread_tcb_t* currTCBPointer = &currTCB;
    queue_enqueue(globalQueue, currTCBPointer);



    //uthread_ctx_switch(uthread_current()->uctx, currThread->uctx);

}

void uthread_exit(void)
{
	/* TODO Phase 2 */
}

uthread_tcb* tcb_create(uthread_func_t func, void *arg)
{

    uthread_tcb* z = (struct uthread_tcb*) malloc(sizeof(struct struct uthread_tcb));

    z->top_of_stack = *uthread_ctx_alloc_stack(void);
    z->state = 0;
    z->uctx = (uthread_ctx_t*) malloc(sizeof(uthread_ctx_t));

    getcontext(z->uctx);
    z->uctx->uc_stack.ss_sp   = z->top_of_stack;
    z->uctx->uc_stack.ss_size = STACK_SIZE;
    makecontext(z->uctx, (func_t)func, arg);

    return z;
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	uthread_tcb* tempStruct = tcb_create(func, arg);
	if (tempStruct == NULL){
	    return -1;
	}
	queue_enqueue(globalQueue, tempStruct);
	return 0;

	// Create a thread control block
	// Create a stack using *uthread_ctx_alloc_stack
    // Contain a pointer to
}

int tcb_free(uthread_tcb_t structToFree)
{
    // free up the thread control block object
}

int uthread_start(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
    globalQueue = queue_create();
    if (uthread_create(idleFunc, void) == -1){ //
        return -1;
    }
    if (uthread_create(func, arg) == -1){ // Create the "main" (second) thread.
        return -1;
    }

    queue_dequeue(globalQueue, (void**) &currTcb);
    uthread_ctx_switch(&mainContext[0], &(*currThread.uctx)); // Something like that.
    // swapcontext(&mainContext[0], &ctx[to]);

    // THIS
    return 0;
    // Create a thread with which to place in the queue. using uthread_create
    //queue_enqueue(GlobalQueue, void *data) enqueue the intial thread into the queue.

	// Initialize the global queue of uthread_tcb objects
	// create the intitial
}

int idleFunc(void){
    while (queue_length(globalQueue) > 1){
        uthread_yield();
        // handle threads with completed execution and terminate their TCB
    }
    // Perform context switch to original thread
    return 0;
}

void uthread_block(void)
{
	/* TODO Phase 2/3 */
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 2/3 */
}

