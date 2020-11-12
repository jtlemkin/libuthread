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

int is_done = -1;

void thread2()
{
    for (int j = 0; j < 5; ++j) {
        printf("Here I am!\n");
        uthread_yield();
    }

    is_done = 1;
}

void thread1()
{
    uthread_create(thread2, NULL);

    while (!is_done)
        ;
}

int main(void)
{
    uthread_start(thread1, NULL);
    return 0;
}