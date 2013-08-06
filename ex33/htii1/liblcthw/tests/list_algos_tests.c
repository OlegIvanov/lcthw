#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>

char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS", "I", "like", "kopro", "experiments", "very", "much"};
#define NUM_VALUES 11

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

char *all_tests()
{
	mu_suite_start();
	
	mu_run_test(test_bubble_sort);
	mu_run_test(test_merge_sort);

	return NULL;
}

RUN_TESTS(all_tests);
