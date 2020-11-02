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
void test_destroy(void)
{
	queue_t q;
	int result;

	fprintf(stderr, "*** TEST destroy simple ***\n");

	q = queue_create();
	result = queue_destroy(q);

	TEST_ASSERT(result == 0);
	TEST_ASSERT(q == NULL);
}
/* Destory Null */
void test_destroy_null(void)
{
	int result;

	fprintf(stderr, "*** TEST destroy null ***\n");

	result = queue_destroy(NULL);

	TEST_ASSERT(result == -1);
}
/* Destroy Not Empty */
void test_destroy_not_empty(void)
{
	queue_t q;
	int result;
	int t;

	fprintf(stderr, "*** TEST destroy not empty ***\n");

	q = queue_create();
	queue_enqueue(q, &t);
	result = queue_destroy(q);

	// Should queue be deleted or not if not empty?

	TEST_ASSERT(result == -1);
}

/* Enqueue null */
void test_enqueue_null(void)
{
	int result;
	int data;

	fprintf(stderr, "*** TEST enqueue null ***\n");

	result = queue_enqueue(NULL, &data);

	TEST_ASSERT(result == -1);
}
/* Enqueue data null */
void test_enqueue_data_null(void)
{
	queue_t q;
	int result;

	fprintf(stderr, "*** TEST enqueue data null ***\n");

	q = queue_create();
	result = queue_enqueue(q, NULL);

	TEST_ASSERT(result == -1);
}
/* Enqueue malloc error */

/* Dequeue null */
void test_dequeue_null(void)
{
	int result;
	int data;
	int *ptr;

	fprintf(stderr, "*** TEST dequeue null ***\n");

	result = queue_dequeue(NULL, (void**)&ptr);

	TEST_ASSERT(result == -1);
}
/* Dequeue data pointer null */
void test_dequeue_data_null(void)
{
	queue_t q;
	int result;

	fprintf(stderr, "*** TEST dequeue data null ***\n");

	q = queue_create();
	result = queue_dequeue(q, NULL);

	TEST_ASSERT(result == -1);
}
/* Dequeue empty */
void test_dequeue_empty(void)
{
	queue_t q;
	int result;
	int data;
	int *ptr;

	fprintf(stderr, "*** TEST dequeue data null ***\n");

	q = queue_create();
	result = queue_dequeue(q, (void**)&ptr);

	TEST_ASSERT(result == -1);
}

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
	int result1, result2;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	result1 = queue_enqueue(q, &data);
	result2 = queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(result1 == 0);
	TEST_ASSERT(result2 == 0);
	TEST_ASSERT(ptr == &data);
}

/* Enqueue/Dequeue complex */
void test_queue_complex(void)
{
	int data[5] = {1, 2, 3, 4, 5};
	int *ptr;
	queue_t q;
	int i;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	for (i = 0; i < 5; ++i) {
		queue_enqueue(q, (data + i));
	}
	for (i = 0; i < 5; ++i) {
		queue_dequeue(q, (void**)&data);
		TEST_ASSERT(ptr == (data + i));
	}
}


int main(void)
{
	test_create();
	test_queue_simple();

	return 0;
}
