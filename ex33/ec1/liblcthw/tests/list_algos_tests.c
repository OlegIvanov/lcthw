#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>
#include <time.h>

char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS", "I", "like", "kopro", "experiments", "very", "much"};
#define NUM_VALUES 11
#define BUBBLE_SORT_ITERATIONS 10000
#define MERGE_SORT_ITERATIONS 1

List *create_words()
{
	int i = 0;
	List *words = List_create();
	
	for(i = 0; i < NUM_VALUES; i++) {
		List_push(words, values[i]);
	}

	return words;
}

char *test_bubble_sort()
{
	List *words = create_words();

	// should work on a list that needs sorting
	int rc = List_bubble_sort(words, (List_compare)strcmp);
	mu_assert(rc == 0, "Bubble sort failed.");
	mu_assert(check_sorting(words, (List_compare)strcmp), "Words are not sorted after bubble sort.");

	// should work on already sorted list
	rc = List_bubble_sort(words, (List_compare)strcmp);
	mu_assert(rc == 0, "Bubble sort of already sorted failed.");
	mu_assert(check_sorting(words, (List_compare)strcmp), "Words should be sort if already bubble sorted.");

	List_clear_destroy(words);

	// should work on an empty list
	words = List_create(words);
	rc = List_bubble_sort(words, (List_compare)strcmp);
	mu_assert(rc == 0, "Bubble sort failed on empty list.");
	mu_assert(check_sorting(words, (List_compare)strcmp), "Words should be sorted if empty.");

	List_clear_destroy(words);

	return NULL;
}

char *test_merge_sort()
{
	List *words = create_words();

	// should work on a list that needs sorting
	List *res = List_merge_sort(words, (List_compare)strcmp);
	mu_assert(check_sorting(res, (List_compare)strcmp), "Words are not sorted after merge sort.");

	List *res2 = List_merge_sort(res, (List_compare)strcmp);
	mu_assert(check_sorting(res, (List_compare)strcmp), "Should still be sorted after merge sort.");

	if(res2 != res) List_clear_destroy(res2);
	if(res != words) List_clear_destroy(res);

	List_clear_destroy(words);

	return NULL;
}

char *compare_perfomance()
{
	struct timespec start, end;
	double diff;

	int i = 0;

	List *bubble_words[BUBBLE_SORT_ITERATIONS];
	int rc[BUBBLE_SORT_ITERATIONS];
	/*
	List *merge_words = NULL;
	List *merged_words[MERGE_SORT_ITERATIONS];
	*/
	// bubble sort bootstrap
	for(i = 0; i < BUBBLE_SORT_ITERATIONS; i++) {
		bubble_words[i] = create_words();
	}

	// bubble sort measuring
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

	for(i = 0; i < BUBBLE_SORT_ITERATIONS; i++) {
		rc[i] = List_bubble_sort(bubble_words[i], (List_compare)strcmp);
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

	diff = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec ) / BUBBLE_SORT_ITERATIONS;
	printf("\nBubble sort took %lf nanoseconds to run.\n\n", diff);

	// bubble sort checking results and freeing of resources
	for(i = 0; i < BUBBLE_SORT_ITERATIONS; i++) {
		mu_assert(rc[i] == 0, "Bubble sort failed.");
		mu_assert(check_sorting(bubble_words[i], (List_compare)strcmp), "Words are not sorted after merge sort.");
		List_clear_destroy(bubble_words[i]);
	}

	// merge sort bootstrap
	/*
	merge_words = create_words();
	*/
	
	// merge sort measuring
	/*
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

	for(i = 0; i < MERGE_SORT_ITERATIONS; i++) {
		merged_words[i] = List_merge_sort(merge_words, (List_compare)strcmp);
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

	diff = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec ) / MERGE_SORT_ITERATIONS;
	printf("Merge sort took %lf nanoseconds to run.\n\n", diff);
	*/
	// merge sort checking results and freeing of resources
	/*
	List_clear_destroy(merge_words);
	for(i = 0; i < MERGE_SORT_ITERATIONS; i++) {
		mu_assert(check_sorting(merged_words[i], (List_compare)strcmp), "Words are not sorted after merge sort.");
		List_clear_destroy(merged_words[i]);
	}
	*/

	return NULL;
}

char *all_tests()
{
	mu_suite_start();
	
	mu_run_test(test_bubble_sort);
	mu_run_test(test_merge_sort);

	mu_run_test(compare_perfomance);

	return NULL;
}

RUN_TESTS(all_tests);
