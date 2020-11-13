/*
 * Premption test
 * 
 * Tests that a cpu hogging thread can be prempted and allow
 * another thread to output
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <uthread.h>

int *is_done;

void thread2()
{
    fprintf(stderr, "*** Thread 2 initiate ***\n");
    fprintf(stderr, "*** Thread 2 value of is_done: %d***\n", *is_done);
    for (int j = 0; j < 5; ++j) {
        fprintf(stderr, "*** Thread 2 inside loop, about to yield ***\n");
        uthread_yield();
        fprintf(stderr, "*** Thread 2 resumes ***\n");
    }
    fprintf(stderr, "*** Thread 2 concluding loop ***\n");

    *is_done = 1;
    fprintf(stderr, "*** Thread 2 value of is_done: %d***\n", *is_done);
    fprintf(stderr, "*** Thread 2 sets value to terminate other thread. ***\n");
}

void thread1()
{
    fprintf(stderr, "*** Thread 1 initiate ***\n");
    uthread_create(thread2, NULL);
    fprintf(stderr, "*** Thread 1 about to enter infinite loop ***\n");
    fprintf(stderr, "*** Thread 1 value of is_done: %d***\n", *is_done);
    int localVal = *is_done;
    while (!localVal){
        localVal = *is_done;
        fprintf(stderr, "*** Thread 1 value of is_done: %d***\n", *is_done);
    }
    fprintf(stderr, "*** Thread 1 concludes due to preemption causing thread 2 to run and change condition to exit while loop ***\n");
}

int main(void)
{
    is_done = malloc(sizeof(int));
    *is_done = 0;
    fprintf(stderr, "*** Main Start ***\n");
    uthread_start(thread1, NULL);
    fprintf(stderr, "*** Main Finish ***\n");
    free(is_done);
    return 0;
}