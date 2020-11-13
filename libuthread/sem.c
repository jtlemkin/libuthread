#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
    // A semaphore should consist of an internal count, and a queue to store
    size_t res_count;
    queue_t blocked_threads;

};

sem_t sem_create(size_t count) {
    // Will need to malloc data for the queue.
    struct semaphore *sem = (struct semaphore *) malloc(sizeof(struct semaphore));
    sem->res_count = count;
    sem->blocked_threads = queue_create();
    return sem;
}

int sem_destroy(sem_t sem) {
    if (!sem) {
        // Semaphore is null. Cannot perform this action
        return -1;
    }
    preempt_disable();
    if (queue_length(sem->blocked_threads) >= 1) {
        preempt_enable();
        return -1;
    }
    queue_destroy(sem->blocked_threads);
    free(sem);
    preempt_enable();
    return 0;
}

int sem_down(sem_t sem) {
    assert(sem->res_count >= 0);

    preempt_disable();
    // Critical section
    if (sem->res_count >= 1) { // If resources are available, continue.
        sem->res_count = sem->res_count - 1;
        preempt_enable();
        return 0;
    }

    // We now have no need to change the internal count.
    // If we reach this point, we should enqueue the TCB to the blocked queue,
    // block the thread and yield.
    queue_enqueue(sem->blocked_threads, uthread_current());

    // Block should perform the yield, and remove the TCB from the main queue.
    uthread_block();
    // Thread should continue running from here when unblocked, and should now 
    // have access to the resource.
    // We could call sem_down(sem) here if we wanted to perform a check again, 
    // but we should instead handle sem_up and unblocking carefully in order to 
    // prevent starvation of this thread.
    return 0;
}

int sem_up(sem_t sem) {
    if (sem == NULL) {
        return -1;
    }
    preempt_disable();
    // Critical section
    sem->res_count += 1;
    if (queue_length(sem->blocked_threads) >= 1) {
        struct uthread_tcb *released_thread;
        // Dequeue the TCB into currTCB
        queue_dequeue(sem->blocked_threads, (void **) &released_thread);
        // Unblock the oldest TCB in the queue of blocked TCBs. 
        // Unblock should place the TCB back into the main queue.
        uthread_unblock(released_thread);
        // This is where the magic happens. Access to the semaphore is RESERVED 
        // for the blocked thread placed back into the queue. If another thread 
        // attempts to get access to the reserved resource, they will be placed 
        // in the semaphore queue in order to access it later.
        sem->res_count -= 1;
    }
    preempt_enable();
    return 0;

}

