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

int queue_free(queue_t q) {
    int *data;

    while(queue_dequeue(q, (void**)&data) == 0)
        ;

    return queue_destroy(q);
}

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

    queue_free(q);
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
    queue_free(q);

    TEST_ASSERT(result == -1);
}
/* Enqueue malloc error */

/* Dequeue null */
void test_dequeue_null(void)
{
    int result;
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
    queue_free(q);

    TEST_ASSERT(result == -1);
}
/* Dequeue empty */
void test_dequeue_empty(void)
{
    queue_t q;
    int result;
    int *ptr;

    fprintf(stderr, "*** TEST dequeue queue empty ***\n");

    q = queue_create();
    result = queue_dequeue(q, (void**)&ptr);
    queue_free(q);

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
void test_delete_data_null(void)
{
    queue_t q;
    int result;

    fprintf(stderr, "*** TEST delete data null ***\n");
    q = queue_create();

    result = queue_delete(q, NULL);
    queue_free(q);

    TEST_ASSERT(result == -1);
}
/* Delete data not in queue */
void test_delete_not_found(void)
{
    int data[] = {1, 2, 3, 4, 5, 6};
    queue_t q;
    int i, result;

    fprintf(stderr, "*** TEST delete not found ***\n");

    q = queue_create();
    for (i = 0; i < 5; ++i) {
        queue_enqueue(q, data + i);
    }

    result = queue_delete(q, data + 5);
    queue_free(q);

    TEST_ASSERT(result == -1);
}
/* Delete success simple */
void test_delete_simple(void)
{
    int data = 3;
    queue_t q;

    fprintf(stderr, "*** TEST delete simple ***\n");

    q = queue_create();
    queue_enqueue(q, &data);

    queue_delete(q, &data);

    TEST_ASSERT(queue_length(q) == 0);

    queue_free(q);
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

    queue_free(q);
}
/* Delete success head */
void test_delete_head(void)
{
    int data[] = {1, 2, 3};
    queue_t q;
    int i;
    int *ptr;

    fprintf(stderr, "*** TEST delete head ***\n");

    q = queue_create();
    for (i = 0; i < 3; ++i) {
        queue_enqueue(q, data + i);
    }

    queue_delete(q, data);
    TEST_ASSERT(queue_length(q) == 2);

    queue_dequeue(q, (void**)&ptr);
    TEST_ASSERT(ptr == data + 1);

    queue_free(q);
}

/* Delete success tail */
void test_delete_tail(void)
{
    int data[] = {1, 2, 3};
    queue_t q;
    int i;
    int *ptr;

    fprintf(stderr, "*** TEST delete tail ***\n");

    q = queue_create();
    for (i = 0; i < 3; ++i) {
        queue_enqueue(q, data + i);
    }

    queue_delete(q, data + 2);
    TEST_ASSERT(queue_length(q) == 2);

    queue_dequeue(q, (void**)&ptr);
    TEST_ASSERT(ptr == data);

    queue_free(q);
}

/* Iterate null */
void inc(void *n) {
    int *a = (int*) n;
    *a = *a + 1;
}

void test_iterate_null(void)
{
    int result;

    fprintf(stderr, "\n*** TEST iterate queue null ***\n\n");

    result = queue_delete(NULL, &inc);

    TEST_ASSERT(result == -1);
}
/* Iterate function null */
void test_iterate_function_null(void)
{
    queue_t q;
    int result;

    fprintf(stderr, "\n*** TEST iterate function null ***\n\n");
    q = queue_create();

    result = queue_delete(q, NULL);
    queue_free(q);

    TEST_ASSERT(result == -1);
}
/* Iterate simple */
void test_iterate_simple(void)
{
    queue_t q;
    int result;
    int data[] = {1, 2, 3};
    int i;
    int *ptr;

    fprintf(stderr, "\n*** TEST iterate simple ***\n\n");

    q = queue_create();
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
    queue_free(q);
}

queue_t q;

/* Callback function that increments items */
static void inc_item(void *data)
{
    int *a = (int*)data;
    if (*a == 5)
        queue_delete(q, data);
    else
        *a += 1;
}

void test_iterator(void)
{
    fprintf(stderr, "\n*** TEST iterator ***\n\n");
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Increment every item of the queue, delete item '5' */
    queue_iterate(q, inc_item);
    TEST_ASSERT(data[0] == 2);
    TEST_ASSERT(queue_length(q) == 9);
    queue_free(q);
}

queue_t global_q;

/*void print_add(void* ptr) {
    printf("add: %p\n", ptr);
}*/

void delete_head(void* ptr) {
    //printf("ptr: %p\n", ptr);
    int *a = (int*) ptr;
    //printf("a: %d\n", *a);
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
void test_iterate_delete(queue_func_t func)
{
    int result;
    int data[] = {1, 2, 3};
    int i, j;
    int *ptr;

    // Test each of the delete functions
    for (j = 0; j < 4; ++j) {
        global_q = queue_create();

        for (i = 0; i < 3; ++i) {
            queue_enqueue(global_q, &(data[i]));
        }

        //queue_iterate(global_q, &print_add);
        queue_iterate(global_q, func);

        for (i = 0; i < 2; ++i) {
            result = queue_dequeue(global_q, (void**)&ptr);

            TEST_ASSERT(result == 0);
        }
        queue_free(global_q);
    }
}

void test_iterate_delete_head(void) {
    fprintf(stderr, "*** TEST iterate delete HEAD ***\n");
    //test_iterate_delete(&delete_head);
}

void test_iterate_delete_tail(void){
    fprintf(stderr, "*** TEST iterate delete TAIL ***\n");
    test_iterate_delete(&delete_tail);
}

void test_iterate_delete_mid(void) {
    fprintf(stderr, "*** TEST iterate delete MID ***\n");
    test_iterate_delete(&delete_mid);
}

void test_iterate_delete_next(void) {
    fprintf(stderr, "*** TEST iterate delete NEXT ***\n");
    test_iterate_delete(&delete_next);
}

/* Length null */
void test_length_null(void)
{
    int result;

    fprintf(stderr, "*** TEST length null ***\n");

    result = queue_length(NULL);

    TEST_ASSERT(result == -1);
}

/* Length simple */
void test_length_simple(void)
{
    queue_t q;
    int data[] = {1, 2, 3};
    int i;

    fprintf(stderr, "*** TEST length simple ***\n");

    q = queue_create();
    for (i = 0; i < 3; ++i) {
        queue_enqueue(q, data + i);
    }
    TEST_ASSERT(queue_length(q) == 3);
    queue_free(q);
}

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
    queue_free(q);
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
        queue_dequeue(q, (void**)&ptr);
        TEST_ASSERT(ptr == (data + i));
    }

    queue_free(q);
}


int main(void)
{
    /*test_create();
    test_destroy();
    test_destroy_null();
    test_destroy_not_empty();
    test_enqueue_null();
    test_enqueue_data_null();
    test_dequeue_null();
    test_dequeue_data_null();
    test_dequeue_empty();
    test_delete_null();
    test_delete_data_null();
    test_delete_not_found();
    test_delete_simple();
    test_delete_complex();
    test_delete_head();
    test_delete_tail();
    test_iterate_null();
    test_iterate_simple();
    test_iterate_function_null();*/
    test_iterator();
    /*test_iterate_delete_head();
    test_iterate_delete_tail();
    test_iterate_delete_mid();
    test_iterate_delete_next();
    test_length_null();
    test_length_simple();
    test_queue_simple();
    test_queue_complex();*/

    return 0;
}
