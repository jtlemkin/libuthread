#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "context.c"
#include "uthread.h"
#include "queue.h"

// General idea. Create a queue of thread objects.

#define STACK_SIZE 32768

typedef void (*func_t) ();

struct uthread_tcb* currTCB;

static uthread_ctx_t mainContext;
queue_t globalQueue;
queue_t exited_threads;

/*
 * An example of information contained within a TCB is:
 *
 * Thread Identifier: Unique id (tid) is assigned to every new thread
 * Stack pointer: Points to thread's stack in the process
 * Program counter: Points to the current program instruction of the thread
 * State of the thread (running, ready, waiting, start, done)
 * Thread's register values
 * Pointer to the Process control block (PCB) of the process that the thread lives on
*/

// A state
// A backup of registers
// a stack
struct uthread_tcb {
	int state;
	uthread_ctx_t *uctx;
    void *top_of_stack;
};

void print_add(void* data) {
    printf("ADD %p\n", data);
}

struct uthread_tcb *uthread_current(void)
{
	return currTCB;
}

void uthread_yield(void)
{
	// General idea: Dequeue the next thread on the

    /*print_add(currTCB);
    queue_iterate(globalQueue, print_add);
    printf("\n");*/

    //printf("YIELD\n");

	struct uthread_tcb* prevThread;
    //printf("uthread_yield_d\n");
    preempt_disable();
	prevThread = currTCB;
	if (queue_dequeue(globalQueue, (void**) &currTCB) == -1) {
        //printf("uthread_yield_e\n");
        preempt_enable();
        return;
    }
    //printf("YIELD DEQUEUE\n: %p", currTCB);
	queue_enqueue(globalQueue, prevThread);
    //preempt_enable();
    //queue_iterate(globalQueue, print_add);
    //printf("YIELD ENQUEUE\n: %p", prevThread);
    // In case of global_queue being empty, prevThread and currTCB point
    // to the same thread and no conta
    uthread_ctx_switch(prevThread->uctx, currTCB->uctx);
    preempt_enable();
}

void uthread_exit(void)
{
    //printf("THREAD EXIT\n");

    struct uthread_tcb* prevThread;
    //printf("uthread_exit_d\n");
    preempt_disable();
	prevThread = currTCB;
	queue_dequeue(globalQueue, (void**) &currTCB);
    queue_enqueue(exited_threads, prevThread);
    //preempt_enable();

    uthread_ctx_switch(prevThread->uctx, currTCB->uctx);
    preempt_enable();
}

struct uthread_tcb *tcb_create(uthread_func_t func, void *arg)
{
    struct uthread_tcb *z = (struct uthread_tcb*) malloc(sizeof(struct uthread_tcb));
    z->top_of_stack = uthread_ctx_alloc_stack();
    z->state = 0;
    z->uctx = (uthread_ctx_t*) malloc(sizeof(uthread_ctx_t));
    uthread_ctx_init(z->uctx, z->top_of_stack, func, arg);

    return z;
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb *tempStruct = tcb_create(func, arg);
	if (tempStruct == NULL){
	    return -1;
	}
    //printf("uthread_create_d\n");
    preempt_disable();
	queue_enqueue(globalQueue, tempStruct);
    //printf("uthread_create_e\n");
    preempt_enable();
	return 0;

	// Create a thread control block
	// Create a stack using *uthread_ctx_alloc_stack
    // Contain a pointer to
}

void tcb_free(struct uthread_tcb *structToFree)
{
    // free up the thread control block object
    uthread_ctx_destroy_stack(structToFree->top_of_stack);
    free(structToFree->uctx);
    free(structToFree);
}

void idleFunc(void *arg){
    struct uthread_tcb *exited_thread;

    while (queue_length(globalQueue) > 0){
        // Clean up exited threads during execution

        // This maybe doesn't need to stop preemption because the idlethread is
        // the only thread that interacts with with the exited_threads queue
        // once preemption is enabled
        while(queue_length(exited_threads) >= 1) {
            preempt_disable();
            queue_dequeue(exited_threads, (void**) &exited_thread);
            preempt_enable();
            tcb_free(exited_thread);
        }
        /*while (queue_dequeue(exited_threads, (void**) &exited_thread) == 0) {
            tcb_free(exited_thread);
        }*/
        uthread_yield();
        // handle threads with completed execution and terminate their TCB
    }

    // Clean up any remaining threads
    while (queue_dequeue(exited_threads, (void**) &exited_thread) == 0) {
        tcb_free(exited_thread);
    }
}

int uthread_start(uthread_func_t func, void *arg)
{
    globalQueue = queue_create();
    exited_threads = queue_create();
    if (uthread_create(idleFunc, (void*) NULL) == -1){ //
        return -1;
    }
    if (uthread_create(func, arg) == -1){ // Create the "main" (second) thread.
        return -1;
    }

    queue_dequeue(globalQueue, (void**) &currTCB);

    preempt_start();

    // Our threads start running after this call
    uthread_ctx_switch(&mainContext, currTCB->uctx); // Something like that.
    // swapcontext(&mainContext[0], &ctx[to]);

    preempt_stop();
    // THIS
    return 0;
    // Create a thread with which to place in the queue. using uthread_create
    //queue_enqueue(GlobalQueue, void *data) enqueue the intial thread into the queue.

	// Initialize the global queue of uthread_tcb objects
	// create the intitial
}

void uthread_block(void)
{
	// Should "yield" but in a manner that it is NOT adding this function to the tail of the queue.
    struct uthread_tcb* prevThread;
    prevThread = currTCB;
    queue_dequeue(globalQueue, (void**) &currTCB);
    //queue_iterate(globalQueue, print_add);
    //printf("YIELD ENQUEUE\n: %p", prevThread);

    //printf("BLOCKED THREAD: \n");
    //print_add(prevThread);
    //printf("NEW QUEUE: \n");
    //queue_iterate(globalQueue, print_add);
    //printf("\n");
    //preempt_enable();
    preempt_disable();
    uthread_ctx_switch(prevThread->uctx, currTCB->uctx); // Change execution. The thread that was blocked has been removed from the queue.
    preempt_enable();
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	// enqueue a TCB that was previously removed.
    queue_enqueue(globalQueue, uthread);
}

