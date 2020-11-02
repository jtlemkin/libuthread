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

	fprintf(stderr, "*** TEST dequeue queue empty ***\n");

	q = queue_create();
	result = queue_dequeue(q, (void**)&ptr);

	TEST_ASSERT(result == -1);
}

/* Delete null */
void test_delete_null(void)
{
	int result;
	int data;

	fprintf(stderr, "*** TEST delete queue null ***\n");

	result = queue_delete(NULL, &data);

	TEST_ASSERT(result == -1);
}
/* Delete data null */
void test_data_null(void)
{
	queue_t q;
	int result;

	fprintf(stderr, "*** TEST delete data null ***\n");

	result = queue_delete(q, NULL);

	TEST_ASSERT(result == -1);
}
/* Delete data not in queue */
void test_delete_not_found(void)
{
	int data[] = {1, 2, 3, 4, 5, 6};
	int *ptr;
	queue_t q;
	int i, result;

	fprintf(stderr, "*** TEST delete not found ***\n");

	q = queue_create();
	for (i = 0; i < 5; ++i) {
		queue_enqueue(q, data + i);
	}
	
	result = queue_delete(q, data + 5);

	TEST_ASSERT(result == -1);
}
/* Delete success simple */
void test_delete_simple(void)
{
	int data = 3;
	queue_t q;
	int i, result;

	fprintf(stderr, "*** TEST delete simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	
	result = queue_delete(q, &data);

	TEST_ASSERT(queue_length(q) == 0);
}
/* Delete success complex */
void test_delete_complex(void)
{
	int data[] = {1, 2, 3, 4, 5};
	queue_t q;
	int i, result;

	fprintf(stderr, "*** TEST delete complex ***\n");

	q = queue_create();
	for (i = 0; i < 5; ++i) {
		queue_enqueue(q, data + i);
	}
	
	result = queue_delete(q, data + 3);

	TEST_ASSERT(queue_length(q) == 4);
	TEST_ASSERT(result == 0);
}
/* Delete success head */
void test_delete_head(void)
{
	int data[] = {1, 2, 3};
	queue_t q;
	int i, result;
	int *ptr;

	fprintf(stderr, "*** TEST delete head ***\n");

	q = queue_create();
	for (i = 0; i < 3; ++i) {
		queue_enqueue(q, data + i);
	}
	
	result = queue_delete(q, data);
	queue_dequeue(q, (void**)&ptr);

	TEST_ASSERT(queue_length(q) == 2);
	TEST_ASSERT(ptr == data + 1);
}

/* Delete success tail */
void test_delete_tail(void)
{
	int data[] = {1, 2, 3};
	queue_t q;
	int i, result;
	int *ptr;

	fprintf(stderr, "*** TEST delete tail ***\n");

	q = queue_create();
	for (i = 0; i < 3; ++i) {
		queue_enqueue(q, data + i);
	}
	
	result = queue_delete(q, data + 2);
	queue_dequeue(q, (void**)&ptr);

	TEST_ASSERT(queue_length(q) == 2);
	TEST_ASSERT(ptr == data);
}

/* Iterate null */
void inc(void *n) {
	int *a = (int*) a;
	*a += 1;
}

void test_iterate_null(void)
{
	int result;

	fprintf(stderr, "*** TEST iterate queue null ***\n");

	result = queue_delete(NULL, &inc);

	TEST_ASSERT(result == -1);
}
/* Iterate function null */
void test_iterate_function_null(void)
{
	queue_t q;
	int result;

	fprintf(stderr, "*** TEST iterate function null ***\n");

	result = queue_delete(q, NULL);

	TEST_ASSERT(result == -1);
}
/* Iterate simple */
void test_iterate_function_null(void)
{
	queue_t q;
	int result;
	int data[] = {1, 2, 3};
	int i;
	int *ptr;

	fprintf(stderr, "*** TEST iterate simple ***\n");
	for (i = 0; i < 3; ++i) {
		queue_enqueue(q, data + i);
	}
	queue_iterate(q, &inc);
	for (i = 0; i < 3; ++i) {
		result = queue_dequeue(q, (void**)&ptr);
		TEST_ASSERT(result == 0);
		// Each element should be incrememented by the inc function, each 
		// element is also originally one greater than it's index, so the new 
		// value should be i + 2
		TEST_ASSERT(*ptr == i + 2);
	}
}

queue_t global_q;

void delete_head(void* ptr) {
	int *a = (int*) ptr;
	if (*a == 1) {
		queue_delete(global_q, a);
	}
}

void delete_mid(void* ptr) {
	int *a = (int*) ptr;
	if (*a == 2) {
		queue_delete(global_q, a);
	}
}

void delete_tail(void* ptr) {
	int *a = (int*) ptr;
	if (*a == 3) {
		queue_delete(global_q, a);
	}
}

void delete_next(void* ptr) {
	int *a = (int*) ptr;
	if (*a == 1) {
		queue_delete(global_q, a + 1);
	}
}

/* Iterate delete element */
void test_iterate_function_null(void)
{
	int result;
	int data[] = {1, 2, 3};
	int i, j;
	int *ptr;
	queue_func_t delete_funcs[] = {delete_head, delete_mid, delete_tail, delete_next};

	fprintf(stderr, "*** TEST iterate delete ***\n");
	// 
	for (j = 0; j < 4; ++j) {
		global_q = queue_create();
		
		for (i = 0; i < 3; ++i) {
			queue_enqueue(global_q, data + i);
		}

		queue_iterate(global_q, delete_funcs + j);

		for (i = 0; i < 3; ++i) {
			result = queue_dequeue(global_q, (void**)&ptr);
			TEST_ASSERT(result == 0);
			// Each element should be incrememented by the inc function, each 
			// element is also originally one greater than it's index, so the new 
			// value should be i + 2
			TEST_ASSERT(*ptr == i + 2);
		}
		queue_destroy(global_q);
	}
}

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
