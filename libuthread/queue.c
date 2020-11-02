#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"


struct queueNode {
    /* TODO Phase 1 */
    int data; // Perhaps we need to replace this at a later date? Should be fine for now.
    queue* nextNode;
};

struct queue {
	/* TODO Phase 1 */
	// Enqueue adds to tail, dequeue removes head and outputs data via passed pointer.
	queueNode* headNode;
    queueNode* tailNode;
};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	// Will need to malloc data for the queue.
	struct queue* Cue = (struct queue*) malloc* sizeof((struct queue));
	Cue -> headNode = NULL;
	Cue -> tailNode = NULL;
	return Cue;

	//TODO: Return "NULL" if malloc fails

}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}

