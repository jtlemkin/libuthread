#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
    // A semaphore should consist of an internal count, and a queue to store
	/* TODO Phase 3 */
	int intCount; // should be size_t?
    queue_t semQueueBlockedTCBs;

};

sem_t sem_create(size_t count) // Potential unhandled problem with sem_create(NULL)?
{
    // Will need to malloc data for the queue.
    struct semaphore* newSemaphore = (struct semaphore*) malloc(sizeof(struct semaphore));
    newSemaphore->intCount = count;
    newQueue->semQueuBlockedTCBs = queue_create();
    return newSemaphore;
}

int sem_destroy(sem_t sem)
{
    if (sem == null){
        return -1; // Semaphore is null. Cannot perform this action
    }
    if (queue_length(sem->semQueueBlockedTCBs) >= 1){
        return -1; // Different problem. Still have blocked threads.
    }
    queue_destroy(sem->semQueueBlockedTCBs);
    free(sem);
    return 0;
}

int sem_down(sem_t sem)
{
    preempt_disable(void);
    // Critical section
	if (sem->intCount >= 1){ // If resources are available, continue.
	    sem->intCount = sem->intCount - 1;
	    return 0;
	}
    preempt_enable(void);


	// We now have no need to change the internal count.
	// If we reach this point, we should enqueue the TCB to the blocked queue, block the thread and yield.
	queue_enqueue(sem->semQueueBlockedTCBs, uthread_current());
    uthread_block(void); // Block should perform the yield, and remove the TCB from the main queue.
    // Thread should continue running from here when unblocked, and should now have access to the resource.

    // We could call sem_down(sem) here if we wanted to perform a check again, but we should instead handle sem_up and unblocking carefully in order to prevent starvation of this thread.
    return 0;
}

int sem_up(sem_t sem)
{
	if (sem == NULL){
	    return -1;
	}
    preempt_disable(void);
    // Critical section
    sem->intCount += 1;
	if (queue_length(sem->semQueueBlockedTCBs) >= 1){
        struct uthread_tcb* unblockedTCB;
        queue_dequeue(sem->semQueueBlockedTCBs, (void**) &unblockedTCB); // Dequeue the TCB into currTCB
        uthread_unblock(unblockedTCB); // Unblock the oldest TCB in the queue of blocked TCBs. Unblock should place the TCB back into the main queue.
        sem->intCount -= 1; // This is where the magic happens. Access to the semaphore is RESERVED for the blocked thread placed back into the queue. If another thread attempts to get access to the reserved resource, they will be placed in the semaphore queue in order to access it later.
	}
    preempt_enable(void);
	return 0;

}

