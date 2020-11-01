#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Create Malloc Error */

/* Destory Simple */
/* Destory Null */
/* Destroy Not Empty */

/* Enqueue null */
/* Enqueue data null */
/* Enqueue empty queue */

/* Dequeue null */
/* Dequeue data pointer null */
/* Dequeue empty */

/* Delete null */
/* Delete data null */
/* Delete data not in queue */
/* Delete success */

/* Iterate null */
/* Iterate simple */
/* Iterate delete element */

/* Length null */
/* Length simple */

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

/* Enqueue/Dequeue complex */




int main(void)
{
	test_create();
	test_queue_simple();

	return 0;
}
