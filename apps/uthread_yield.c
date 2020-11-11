/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

void thread4(void *arg)
{
	uthread_yield();
	printf("thread4\n");
}

void thread3(void *arg)
{
	//printf("thread3_start\n");
	uthread_create(thread4, NULL);
	//printf("thread3_yield\n");
	uthread_yield();
	printf("thread3\n");
	//printf("thread3_exit\n");
}

void thread2(void *arg)
{
	//printf("thread2_start\n");
	uthread_create(thread3, NULL);
	//printf("thread2_yield\n");
	uthread_yield();
	printf("thread2\n");
	//printf("thread2_exit\n");
}

void thread1(void *arg)
{
	//printf("thread1_start\n");
	uthread_create(thread2, NULL);
	//printf("thread1_yield\n");
	uthread_yield();
	printf("thread1\n");
	uthread_yield();
	//printf("thread1_exit\n");
}

int main(void)
{
	uthread_start(thread1, NULL);
	return 0;
}
