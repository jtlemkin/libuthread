#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"


struct queue_node {
    void* data;
    struct queue_node* next;
};

struct queue {
    // Enqueue adds to tail, dequeue removes head and outputs data via passed 
    // pointer.
    struct queue_node* head_node;
    struct queue_node* tail_node;
    int length;
};

queue_t queue_create(void)
{
    // Will need to malloc data for the queue.
    struct queue* new_queue = (struct queue*) malloc(sizeof(struct queue));
    new_queue->head_node = NULL;
    new_queue->tail_node = NULL;
    new_queue->length = 0;
    return new_queue;

    // If malloc fails new_queue should be null. (Malloc returns null)
}

int queue_destroy(queue_t queue)
{

    if (queue == NULL || queue->length != 0){
        return -1; // Queue object is not correct
    }

    free(queue);
    return 0; // Success.
}

struct queue_node *node_create(void* data) {
    // Create a new node:
    struct queue_node* node = (struct queue_node*) malloc(sizeof(struct queue_node));
    if (node == NULL){
        // Malloc failed.
        return NULL;
    }
    // Set the data pointer of the node to point at the address of the input data.
    node->data = data;
    node->next = NULL;

    return node;
}

int queue_enqueue(queue_t queue, void *data)
{
    if (queue == NULL || data == NULL){
        return -1; // Queue object is not correct
    }

    // Create a new node:
    struct queue_node* newNode = node_create(data);

    if (!newNode) {
        // Malloc issue
        return -1;
    }

    if (queue->length == 0) {
        // If we need to initialize the queue with the first bit of data, we 
        // have to set both the head and tail equal to the same new node.
        queue->tail_node = newNode;
        queue->head_node = newNode;
    } else {
        // If containing nodes already, set the tail's next node to be equal to 
        // the newly created node, then set the new node as the new tail
        queue->tail_node->next = newNode;
        queue->tail_node = newNode;
    }

    queue->length += 1;

    return 0; // Success

}

int queue_dequeue(queue_t queue, void **data)
{
    if (queue == NULL || data == NULL || queue->length == 0){
        return -1; // Cannot dequeue, queue is null.
    }

    // Update the void pointer to point to the data being stored.
    *data = queue->head_node->data;
    // Store old head temporarily
    struct queue_node* old_head = queue->head_node;
    // Adjust queue to account for removal of the old head
    queue->head_node = queue->head_node->next;
    queue->length -= 1;

    if (queue->length == 0){
        // Seems that was the last node. Set the queue to the empty state
        queue->tail_node = NULL;
    }

    free(old_head); // Delete the old node's data in memory.
    return 0;

}

int queue_delete(queue_t queue, void *data)
{
    // Gameplan: Start at the head of the queue, work towards the tail, 
    // checking to see if the data matches.

    if (!queue || !data || !queue->length){
        return -1; // Cannot dequeue, queue is null.
    }

    struct queue_node* curr_node = queue->head_node;
    struct queue_node* priorNode = NULL;

    while (curr_node != NULL){ // Should handle case of a null queue appropriately.
        if (curr_node->data == data) { 
            if (priorNode == NULL){
                // Case in which the head is the node with data to delete. 
                // Overwrite.
                queue->head_node = queue->head_node->next;

            } else {
                // Adjust queue structure, skipping over the node that we found 
                // the data at.
                priorNode->next = curr_node->next;
                if (priorNode->next == NULL){
                    // Case in which we have just deleted the tail. Set the 
                    // prior node to be the tail.
                    queue->tail_node = priorNode;
                }
            }
            // Given that we have removed the curr_node from the queue, we can 
            // free it.
            free(curr_node);
            queue->length -= 1;
            return 0;
        }
        // If the data was not present at the curr_node we examined, store it as 
        // the previous node we looked at and proceed.
        priorNode = curr_node;
        // Adjust temp node as we move through the linked list
        curr_node = curr_node->next;
    }
    return -1; // Data not found in any of the nodes.
}

int queue_iterate(queue_t queue, queue_func_t func)
{
    if (queue == NULL || func == NULL){
        return -1; // Cannot perform action, queue or func is null.
    }

    struct queue_node* curr_node = queue->head_node;
    struct queue_node* next_node = NULL;
    while (curr_node){
        printf("Curr %p\n", curr_node);
        next_node = curr_node->next; // Store next node prior to editing.
        func(curr_node->data); // Call the function on the data
        printf("Call\n");
        if (curr_node->next) {
            printf("Next %p\n", curr_node->next);
            curr_node = curr_node->next; // Then move on.
        } else {
            printf("No next\n");
            // If there is no longer a next node, this can mean a few things. 
            // We could either have deleted the current node, or we may have 
            // reached the end.
            if (next_node) { 
                // Ah, it seems we deleted the node we were working on. This 
                // could occur if delete is called on the info of curr_node.
                curr_node = next_node;
            } else { 
                // Well it seems there wasn't a next node present before we 
                // called the function either. We must have finished.
                return 0;
            }
        }
    }
    // Shouldn't reach here unless curr_node was null to begin with. In which 
    // case, we have an empty queue.
    return 0;
}

int queue_length(queue_t queue)
{
    if (queue == NULL){
        return -1; // Cannot check length, queue is null
    }
    return queue->length;
}

