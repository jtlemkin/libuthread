#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"


struct queueNode {
    /* TODO Phase 1 */
    void* data; // Perhaps we need to replace this at a later date? Should be fine for now. We need to ensure this can store all data types.
    struct queueNode* nextNode;
};

struct queue {
    /* TODO Phase 1 */
    // Enqueue adds to tail, dequeue removes head and outputs data via passed pointer.
    struct queueNode* headNode;
    struct queueNode* tailNode;
};

queue_t queue_create(void)
{
    /* TODO Phase 1 */
    // Will need to malloc data for the queue.
    struct queue* newQueue = (struct queue*) malloc(sizeof(struct queue));
    newQueue -> headNode = NULL;
    newQueue -> tailNode = NULL;
    return newQueue;

    //TODO: Return "NULL" if malloc fails

}

int queue_destroy(queue_t queue)
{
    /* TODO Phase 1 */
    // Involves freeing up all the data in the queue.
}

int queue_enqueue(queue_t queue, void *data)
{
    /* TODO Phase 1 */

    // Create a new node:
    struct queueNode* newNode = (struct queueNode*) malloc(sizeof(struct queueNode));
    newNode->data = &data; // Set the data pointer of the node to point at the address of the input data. (Only way I can think to handle void* style data input)
    // May need to reference the discussion from last week to ensure this is working properly. Proper approach might be: newNode.data = &data;
    newNode->nextNode = NULL;

    // Then add it to the queue:
    if (queue->headNode != NULL){ // If containing nodes already, set the tail's next node to be equal to the newly created node, then set the new node as the new tail
        queue->tailNode->nextNode = newNode;
        queue->tailNode = newNode;
    } else{ // If we need to initialize the queue with the first bit of data, we have to set both the head and tail equal to the same new node.
        queue->tailNode = newNode;
        queue->headNode = newNode;
    }

    //TODO: appropriate error codes on return.
    return 0;

}

int queue_dequeue(queue_t queue, void **data)
{
    /* TODO Phase 1 */

    if (queue->headNode == NULL){
        return -1; // Cannot dequeue, queue is empty.
    }


    *data = queue->headNode->data; // update the void pointer to point to the data being stored. Somewhat uncertain about this due to void** shennanigans

    struct queueNode* tempHead = queue->headNode; // Store old head temporarily
    queue->headNode = queue->headNode->nextNode; // Adjust queue to account for removal of the old head

    if (queue->headNode == NULL){
        queue->tailNode == NULL; // Seems that was the last node. Set the queue to the empty state
    }

    free(tempHead); // Delete the old node's data in memory.

    return 0;

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

