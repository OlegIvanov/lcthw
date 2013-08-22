#include "minunit.h"
#include <lcthw/darray_algos.h>
#include <time.h>

#define BILLION 1000000000UL

#define ARRAYS_COUNT 1000000

static unsigned long get_diff(struct timespec start, struct timespec end)
{
	return (unsigned long)(end.tv_sec - start.tv_sec) * BILLION + (unsigned long)(end.tv_nsec - start.tv_nsec);
}

int testcmp(char **a, char **b)
{
	return strcmp(*a, *b);
}

DArray *create_words()
{
	DArray *result = DArray_create(100, 15);
	char *words[] = {"asdfasfd", "werwar", "13234", "asdfasfd", "oioj", "trololo", "I", "like", "big", "boobs", 
			"piss", "coming", "from", "my", "ass"};
	int i = 0;

	for(i = 0; i < 15; i++) {
		DArray_push(result, words[i]);
	}

	return result;
}

int is_sorted(DArray *array)
{
	int i = 0;

	for(i = 0; i < DArray_count(array) - 1; i++) {
		if(strcmp(DArray_get(array, i), DArray_get(array, i + 1)) > 0) {
			return 0;
		}
	}

	return 1;
}

char *run_sort_test(int (*func)(DArray *, DArray_compare), const char *name)
{
	DArray *words = create_words();
	mu_assert(!is_sorted(words), "Words should start not sorted.");

	debug("--- Testing %s sorting algorithm", name);
	int rc = func(words, (DArray_compare)testcmp);
	mu_assert(rc == 0, "sort failed");
	mu_assert(is_sorted(words), "didn't sort it");

	DArray_destroy(words);

	return NULL;
}

char *test_qsort()
{
	return run_sort_test(DArray_qsort, "qsort");
}

char *test_heapsort()
{
	return run_sort_test(DArray_heapsort, "heapsort");
}

char *test_mergesort()
{
	return run_sort_test(DArray_mergesort, "mergesort");
}

char *test_quicksort()
{
	return run_sort_test(DArray_quicksort, "quicksort");
}

char *test_algo_perfomance(int (*func)(DArray *, DArray_compare), const char *name)
{
	struct timespec start, end;
	double diff;

	DArray *arrays[ARRAYS_COUNT] = {NULL};

	int i = 0;

	for(i = 0; i < ARRAYS_COUNT; i++) {
		arrays[i] = create_words();
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

	for(i = 0; i < ARRAYS_COUNT; i++) {
		func(arrays[i], (DArray_compare)testcmp);
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

	diff = (double)get_diff(start, end) / ARRAYS_COUNT;
	printf("\nAlgorithm %s took %lf nanoseconds to run.\n\n", name, diff);

	for(i = 0; i < ARRAYS_COUNT; i++) {
		mu_assert(is_sorted(arrays[i]), "didn't sort it");
		DArray_destroy(arrays[i]);
	}

	return NULL;
}

char *test_qsort_perfomance()
{
	return test_algo_perfomance(DArray_qsort, "qsort (original)");
}

char *test_heapsort_perfomance()
{
	return test_algo_perfomance(DArray_heapsort, "heapsort (custom)");
}

char *test_mergesort_perfomance()
{
	return test_algo_perfomance(DArray_mergesort, "mergesort (custom)");
}

char *test_quicksort_perfomance()
{
	return test_algo_perfomance(DArray_quicksort, "quicksort (custom)");
}

char *all_tests()
{
	mu_suite_start();

	mu_run_test(test_qsort);
	mu_run_test(test_heapsort);
	mu_run_test(test_mergesort);
	mu_run_test(test_quicksort);

	mu_run_test(test_qsort_perfomance);
	mu_run_test(test_heapsort_perfomance);
	mu_run_test(test_mergesort_perfomance);
	mu_run_test(test_quicksort_perfomance);

	return NULL;
}

RUN_TESTS(all_tests);
