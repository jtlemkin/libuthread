#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"


struct queueNode {
    void* data;
    struct queueNode* nextNode;
};

struct queue {
    // Enqueue adds to tail, dequeue removes head and outputs data via passed pointer.
    struct queueNode* headNode;
    struct queueNode* tailNode;
    int length;
};

queue_t queue_create(void)
{
    // Will need to malloc data for the queue.
    struct queue* newQueue = (struct queue*) malloc(sizeof(struct queue));
    newQueue->headNode = NULL;
    newQueue->tailNode = NULL;
    newQueue->length = 0;
    return newQueue;

    // If malloc fails newQueue should be null. (Malloc returns null)
}

int queue_dequeueNoCollect(queue_t queue)
{
    if (queue->headNode == NULL){
        return -1; // Cannot dequeue, queue is empty.
    }

    // for debug:
    fprintf(stderr, "*** DEBUG deQueueNoCollect ***\n");
    //fprintf(stderr, "*** %d ***\n", *(queue->headNode->data));

    queue->headNode->data = NULL;

    struct queueNode* tempHead = queue->headNode; // Store old head temporarily
    queue->headNode = queue->headNode->nextNode; // Adjust queue to account for removal of the old head

    if (queue->headNode == NULL){
        queue->tailNode = NULL; // Seems that was the last node. Set the queue to the empty state
    }

    free(tempHead); // Delete the old node's data in memory.
    return 0;

}

int queue_destroy(queue_t queue)
{

    if (queue == NULL || queue->length != 0){
        return -1; // Queue object is not correct
    }

    free(queue);
    return 0; // Success.

/*
 *
 *
    // Involves freeing up all the data in the queue.
    // recursively call dequeue like operation to clear all the nodes contained within the queue, then free the queue.

    int i = 0;
    while (i == 0){
        i = queue_dequeueNoCollect(queue); // Remove nodes (freeing them) until we have none left
    }
    queue->headNode = NULL;
    queue->tailNode = NULL;

    free(queue);

    return 0; // Success.
*/
// Seems we do not need to clear the nodes from the queue if it is not already empty. Misinterpreted the function. Only destroy the queue if it is empty.
}

struct queueNode *node_create(void* data) {
    // Create a new node:
    struct queueNode* node = (struct queueNode*) malloc(sizeof(struct queueNode));
    if (node == NULL){
        // Malloc failed.
        return NULL;
    }
    node->data = data; // Set the data pointer of the node to point at the address of the input data. (Only way I can think to handle void* style data input)
    // May need to reference the discussion from last week to ensure this is working properly. Proper approach might be: newNode.data = &data;
    node->nextNode = NULL;

    return node;
}

int queue_enqueue(queue_t queue, void *data)
{
    if (queue == NULL || data == NULL){
        return -1; // Queue object is not correct
    }

    // Create a new node:
    struct queueNode* newNode = node_create(data);

    if (!newNode) {
        // Malloc issue
        return -1;
    }

    if (queue->length == 0) {
        // If we need to initialize the queue with the first bit of data, we 
        // have to set both the head and tail equal to the same new node.
        queue->tailNode = newNode;
        queue->headNode = newNode;
    } else {
        // If containing nodes already, set the tail's next node to be equal to 
        // the newly created node, then set the new node as the new tail
        queue->tailNode->nextNode = newNode;
        queue->tailNode = newNode;
    }

    queue->length += 1;

    return 0; // Sucess

}

int queue_dequeue(queue_t queue, void **data)
{
    if (queue == NULL || data == NULL || queue->length == 0){
        return -1; // Cannot dequeue, queue is null.
    }

    // for debug:
    fprintf(stderr, "*** In Dequeue ***\n");
    //fprintf(stderr, "*** %d ***\n", &(queue->headNode->data));

    *data = queue->headNode->data; // update the void pointer to point to the data being stored. Somewhat uncertain about this due to void** shennanigans

    struct queueNode* tempHead = queue->headNode; // Store old head temporarily
    queue->headNode = queue->headNode->nextNode; // Adjust queue to account for removal of the old head
    queue->length -= 1;

    if (queue->length == 0){
        queue->tailNode = NULL; // Seems that was the last node. Set the queue to the empty state
    }

    free(tempHead); // Delete the old node's data in memory.
    return 0;

}

int queue_delete(queue_t queue, void *data)
{
    // Gameplan: Start at the head of the queue, work towards the tail, checking to see if the data matches.

    if (queue == NULL || data == NULL || queue->length == 0){
        return -1; // Cannot dequeue, queue is null.
    }

    struct queueNode* tempNode = queue->headNode;
    struct queueNode* priorNode = NULL;

    while (tempNode != NULL){ // Should handle case of a null queue appropriately.
        // I'm pretty sure we want to do a comparison in the following manner: if (*(tempNode->data) == *data) ... But this doesn't seem to work. How do we compare the data of void pointers?
        if (tempNode->data == data){ // I think this should work?
            //fprintf(stderr, "*** %d ***\n", &(tempNode->data));
            fprintf(stderr, "*** Match found, attempting deletion ***\n");
            if (priorNode == NULL){
                queue->headNode = queue->headNode->nextNode; // Case in which the head is the node with data to delete. Overwrite.
            } else {
                priorNode->nextNode = tempNode->nextNode; // Adjust queue structure, skipping over the node that we found the data at.
                if (priorNode->nextNode == NULL){
                    queue->tailNode = priorNode; // Case in which we have just deleted the tail. Set the prior node to be the tail.
                }
            }
            free(tempNode); // Given that we have removed the tempNode from the queue, we can free it.
            queue->length -= 1;
            return 0;
        }
        priorNode = tempNode; // If the data was not present at the tempNode we examined, store it as the previous node we looked at and proceed.
        tempNode = tempNode->nextNode; // Adjust temp node as we move through the linked list
    }
    return -1; // Data not found in any of the nodes.
}

queue_t queue_copy(queue_t queue) {
    queue_t new_queue = queue_create();
    struct queueNode *current = queue->headNode;

    while (current) {
        queue_enqueue(new_queue, current->data);
        current = current->nextNode;
    }

    return new_queue;
}

// Deallocates the memory of a queue even if non-empty
int queue_free(queue_t queue) {
    void *ptr;

    // Empty the queue
    while (queue_dequeue(queue, &ptr) != -1);

    // Then destroy
    return queue_destroy(queue);
}

int queue_iterate(queue_t queue, queue_func_t func)
{
    // Consider making a copy of the queue in order to prevent things from being deleted as you are iterating through it and causing problems.
    // Attempting this method first, will test to see if it functions correctly:

    // A thought: Perhaps it is only necessary to make a copy if we are using a non linked listed implementation? IE, if we had attempted at the start to iterate using the length of the original array as an index
    // We should be fine. Going to test this:

    if (queue == NULL || func == NULL){
        return -1; // Cannot perform action, queue is null.
    }

    // Create a copy so that no matter what, the func is called on every
    // node, even if it is deleted before we get to it
    queue_t copy = queue_copy(queue);
    struct queueNode *current = copy->headNode;

    while (current) {
        func(current->data);
        current = current->nextNode;
    }

    queue_free(copy);

    return 0;
}

int queue_length(queue_t queue)
{
    if (queue == NULL){
        return -1; // Cannot check length, queue is null
    }
    return queue->length;
}

