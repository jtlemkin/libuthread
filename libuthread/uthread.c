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

struct uthread_tcb* curr_thread;

static uthread_ctx_t main_ctx;
queue_t global_queue;
queue_t exited_threads;

/*
 * An example of information contained within a TCB is:
 *
 * Thread Identifier: Unique id (tid) is assigned to every new thread
 * Stack pointer: Points to thread's stack in the process
 * Program counter: Points to the current program instruction of the thread
 * Thread's register values
 * Pointer to the Process control block (PCB) of the process that the thread 
 * lives on
*/

// A backup of registers
// a stack
struct uthread_tcb {
	uthread_ctx_t *uctx;
    void *top_of_stack;
};


struct uthread_tcb *uthread_current(void)
{
	return curr_thread;
}

void uthread_yield(void)
{
	// General idea: Dequeue the next thread on the
	struct uthread_tcb* prevThread;

    preempt_disable();
	prevThread = curr_thread;
	if (queue_dequeue(global_queue, (void**) &curr_thread) == -1) {
        preempt_enable();
        return;
    }

	queue_enqueue(global_queue, prevThread);

    // In case of global_queue being empty, prevThread and curr_thread point
    // to the same thread and no conta
    uthread_ctx_switch(prevThread->uctx, curr_thread->uctx);
    preempt_enable();
}

void uthread_exit(void)
{
    struct uthread_tcb* prevThread;
    preempt_disable();
	prevThread = curr_thread;
	queue_dequeue(global_queue, (void**) &curr_thread);
    queue_enqueue(exited_threads, prevThread);

    uthread_ctx_switch(prevThread->uctx, curr_thread->uctx);
    preempt_enable();
}

struct uthread_tcb *tcb_create(uthread_func_t func, void *arg)
{
    struct uthread_tcb *z = (struct uthread_tcb*) malloc(sizeof(struct uthread_tcb));
    z->top_of_stack = uthread_ctx_alloc_stack();
    z->uctx = (uthread_ctx_t*) malloc(sizeof(uthread_ctx_t));
    uthread_ctx_init(z->uctx, z->top_of_stack, func, arg);

    return z;
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb *new_thread = tcb_create(func, arg);
	if (new_thread == NULL) {
	    return -1;
	}
    preempt_disable();
	queue_enqueue(global_queue, new_thread);
    preempt_enable();
	return 0;
}

void tcb_free(struct uthread_tcb *thread)
{
    // free up the thread control block object
    uthread_ctx_destroy_stack(thread->top_of_stack);
    free(thread->uctx);
    free(thread);
}

void idle(void *arg){
    struct uthread_tcb *exited_thread;

    while (queue_length(global_queue) > 0){
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
    global_queue = queue_create();
    exited_threads = queue_create();
    if (uthread_create(idle, (void*) NULL) == -1) {
        return -1;
    }
    // Create the "main" (second) thread.
    if (uthread_create(func, arg) == -1){
        return -1;
    }

    queue_dequeue(global_queue, (void**) &curr_thread);

    preempt_start();

    // Our threads start running after this call
    uthread_ctx_switch(&main_ctx, curr_thread->uctx);

    preempt_stop();
    return 0;
}

void uthread_block(void)
{
	// Should "yield" but in a manner that it is NOT adding this function to 
    // the tail of the queue.
    struct uthread_tcb* prevThread;
    prevThread = curr_thread;
    queue_dequeue(global_queue, (void**) &curr_thread);

    preempt_disable();
    // Change execution. The thread that was blocked has been removed from the 
    // queue.
    uthread_ctx_switch(prevThread->uctx, curr_thread->uctx);
    preempt_enable();
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	// enqueue a TCB that was previously removed.
    queue_enqueue(global_queue, uthread);
}

