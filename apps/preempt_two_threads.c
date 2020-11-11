/*
 * Premption test
 * 
 * Tests that a cpu hogging thread can be prempted and allow
 * another thread to output
*/

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

void thread2()
{
    for (int i = 0; i < 5; ++i) {
        printf("Here I am!\n");
        uthread_yield();
    }
}

void thread1()
{
    uthread_create(thread2, NULL);

    while (1)
        ;
}

int main(void)
{
    uthread_start(thread1, NULL);
    return 0;
}