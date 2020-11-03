#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"


struct queueNode {
    /* TODO Phase 1 */
    void* data;
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

    // for debug:
    fprintf(stderr, "*** %d ***\n", queue->headNode->data);

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
    // Gameplan: Start at the head of the queue, work towards the tail, checking to see if the data matches.
    struct queueNode* tempNode = queue->headNode;
    struct queueNode* priorNode = NULL;

    while (tempNode != NULL){ // Should handle case of a null queue appropriately.
        // I'm pretty sure we want to do a comparison in the following manner: if (*(tempNode->data) == *data) ... But this doesn't seem to work. How do we compare the data of void pointers?
        if ((tempNode->data) == (data)){ // I think this should work?
            fprintf(stderr, "*** %d ***\n", tempNode->data);
            fprintf(stderr, "*** Match found, attempting deletion ***\n", *(int*)tempNode->data);
            if (priorNode == NULL){
                queue->headNode = queue->headNode->nextNode; // Case in which the head is the node with data to delete. Overwrite.
            } else {
                priorNode->nextNode = tempNode->nextNode; // Adjust queue structure, skipping over the node that we found the data at.
                if (priorNode->nextNode == NULL){
                    queue->tailNode  =  priorNode; // Case in which we have just deleted the tail. Set the prior node to be the tail.
                }
            }
            free(tempNode); // Given that we have removed the tempNode from the queue, we can free it.
            return 0;
        }
        priorNode = tempNode; // If the data was not present at the tempNode we examined, store it as the previous node we looked at and proceed.
        tempNode = tempNode->nextNode; // Adjust temp node as we move through the linked list
    }
    return -1; // Data not found in any of the nodes.
}

int queue_iterate(queue_t queue, queue_func_t func)
{
    /* TODO Phase 1 */
    // Consider making a copy of the queue in order to prevent things from being deleted as you are iterating through it and causing problems.
}

int queue_length(queue_t queue)
{
    int length = 0;
    struct queueNode* tempNode = queue->headNode;
    while (tempNode != NULL){ // Should handle base case of a null queue appropriately.
        length += 1;
        tempNode = tempNode->nextNode; // Adjust temp node as we move through the linked list
    }
    return length;
}

