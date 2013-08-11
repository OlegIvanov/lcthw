#include "minunit.h"
#include <lcthw/darray.h>
#include <lcthw/list.h>
#include <time.h>

static DArray *array = NULL;
static int *val1 = NULL;
static int *val2 = NULL;

#define BILLION					1000000000UL

#define DARRAY_PUSH_ITER		500000000L
#define DARRAY_POP_ITER 		500000000L
#define DARRAY_REMOVE_ITER		500000000L

#define LIST_PUSH_ITER			10000000L
#define LIST_POP_ITER			10000000L
#define LIST_REMOVE_ITER		10000000L

unsigned long get_diff(struct timespec start, struct timespec end)
{
	return (unsigned long)(end.tv_sec - start.tv_sec) * BILLION + (unsigned long)(end.tv_nsec - start.tv_nsec);
}

char *test_create()
{
	array = DArray_create(sizeof(int), 100);
	mu_assert(array != NULL, "DArray_create failed.");
	mu_assert(array->contents != NULL, "contents are wrong in darray");
	mu_assert(array->end == 0, "end isn't at the right spot");
	mu_assert(array->element_size == sizeof(int), "element size is wrong.");
	mu_assert(array->max == 100, "wrong max length on initial size");

	return NULL;
}

char *test_destroy()
{
	DArray_destroy(array);
	
	return NULL;
}

char *test_new()
{
	val1 = DArray_new(array);
	mu_assert(val1 != NULL, "failed to make a new element")

	val2 = DArray_new(array);
	mu_assert(val2 != NULL, "failed to make a new element");

	return NULL;
}

char *test_set()
{
	DArray_set(array, 0, val1);
	DArray_set(array, 1, val2);

	return NULL;
}

char *test_get()
{
	mu_assert(DArray_get(array, 0) == val1, "Wrong first value.");
	mu_assert(DArray_get(array, 1) == val2, "Wrong second value.");

	return NULL;
}

char *test_remove()
{
	int *val_check = DArray_remove(array, 0);
	mu_assert(val_check != NULL, "Should not get NULL.");
	mu_assert(*val_check == *val1, "Should get the first value.");
	mu_assert(DArray_get(array, 0) == NULL, "Should be gone.");
	DArray_free(val_check);

	val_check = DArray_remove(array, 1);
	mu_assert(val_check != NULL, "Should not get NULL.");
	mu_assert(*val_check == *val2, "Should get the first value.")
	mu_assert(DArray_get(array, 1) == NULL, "Should be gone.");
	DArray_free(val_check);

	return NULL;
}

char *test_expand_contract()
{
	int old_max = array->max;
	DArray_expand(array);
	mu_assert((unsigned int)array->max == old_max + array->expand_rate, "Wrong size after expand.");
	
	DArray_contract(array);
	mu_assert((unsigned int)array->max == array->expand_rate + 1, "Should stay at the expand_rate at least.");
	
	DArray_contract(array);
	mu_assert((unsigned int)array->max == array->expand_rate + 1, "Should stay at the expand_rate at least.");

	return NULL;
}

char *test_push_pop()
{
	int i = 0;
	for(i = 0; i < 1000; i++) {
		int *val = DArray_new(array);
		*val = i * 333;
		DArray_push(array, val);
	}

	mu_assert(array->max == 1201, "Wrong max size.");

	for(i = 999; i >= 0; i--) {
		int *val = DArray_pop(array);
		mu_assert(val != NULL, "Shouldn't get a NULL.");
		mu_assert(*val == i * 333, "Wrong value.");
		DArray_free(val);
	}

	return NULL;
}

char *test_darray_push_perfomance()
{
	struct timespec start, end;
	double diff;
	int i = 0;
	char c = 'c';

	DArray *darr = DArray_create(sizeof(char), DARRAY_PUSH_ITER);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	for(i = 0; i < DARRAY_PUSH_ITER; i++) {
		DArray_push(darr, &c);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
	
	diff = (double)get_diff(start, end) / DARRAY_PUSH_ITER;
	printf("\nDArray push took %lf nanoseconds to run.\n", diff);

	DArray_destroy(darr);

	return NULL;
}

char *test_list_push_perfomance()
{
	struct timespec start, end;
	double diff;
	int i = 0;
	char c = 'c';

	List *list = List_create();

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	for(i = 0; i < LIST_PUSH_ITER; i++) {
		List_push(list, &c);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
	
	diff = (double)get_diff(start, end) / LIST_PUSH_ITER;
	printf("\nList push took %lf nanoseconds to run.\n", diff);

	List_clear_destroy(list);

	return NULL;
}

char *test_darray_pop_perfomance()
{
	struct timespec start, end;
	double diff;
	int i = 0;
	char c = 'c';
	char *popped = NULL;

	DArray *darr = DArray_create(sizeof(char), DARRAY_POP_ITER);
	for(i = 0; i < DARRAY_POP_ITER; i++) {
		DArray_push(darr, &c);
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	for(i = 0; i < DARRAY_POP_ITER; i++) {
		popped = DArray_pop(darr);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
	
	diff = (double)get_diff(start, end) / DARRAY_POP_ITER;
	printf("\nDArray pop took %lf nanoseconds to run.\n", diff);

	DArray_destroy(darr);

	return NULL;
}

char *test_list_pop_perfomance()
{
	struct timespec start, end;
	double diff;
	int i = 0;
	char c = 'c';
	char *popped = NULL;

	List *list = List_create();
	for(i = 0; i < LIST_POP_ITER; i++) {
		List_push(list, &c);
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	for(i = 0; i < LIST_POP_ITER; i++) {
		popped = List_pop(list);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
	
	diff = (double)get_diff(start, end) / LIST_POP_ITER;
	printf("\nList pop took %lf nanoseconds to run.\n", diff);

	List_clear_destroy(list);

	return NULL;
}

char *test_darray_remove_perfomance()
{
	struct timespec start, end;
	double diff;
	int i = 0;
	char c = 'c';
	char *removed = NULL;

	DArray *darr = DArray_create(sizeof(char), 1);
	for(i = 0; i < DARRAY_REMOVE_ITER; i++) {
		DArray_push(darr, &c);
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	for(i = DARRAY_REMOVE_ITER - 1; i >= 0; i--) {
		removed = DArray_remove(darr, i);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
	
	diff = (double)get_diff(start, end) / DARRAY_REMOVE_ITER;
	printf("\nDArray remove took %lf nanoseconds to run.\n", diff);

	DArray_destroy(darr);

	return NULL;
}

char *test_list_remove_perfomance()
{
	struct timespec start, end;
	double diff;
	int i = 0;
	char c = 'c';
	ListNode *node_addresses[LIST_REMOVE_ITER] = {NULL};
	char *removed = NULL;

	List *list = List_create();
	for(i = 0; i < LIST_REMOVE_ITER; i++) {
		List_push(list, &c);
		node_addresses[i] = list->last;
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	for(i = LIST_REMOVE_ITER - 1; i >= 0; i--) {
		removed = List_remove(list, node_addresses[i]);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
	
	diff = (double)get_diff(start, end) / LIST_REMOVE_ITER;
	printf("\nList remove took %lf nanoseconds to run.\n", diff);

	List_clear_destroy(list);

	return NULL;
}

char *all_tests() {
	mu_suite_start();

	mu_run_test(test_create);
	mu_run_test(test_new);
	mu_run_test(test_set);
	mu_run_test(test_get);
	mu_run_test(test_remove);
	mu_run_test(test_expand_contract);
	mu_run_test(test_push_pop);
	mu_run_test(test_destroy);

	mu_run_test(test_darray_push_perfomance);
	mu_run_test(test_list_push_perfomance);
	mu_run_test(test_darray_pop_perfomance);
	mu_run_test(test_list_pop_perfomance);
	mu_run_test(test_darray_remove_perfomance);
	mu_run_test(test_list_remove_perfomance);

	return NULL;
}

RUN_TESTS(all_tests);
