#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>

static List *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";
char *test4 = "test4 data";
char *test5 = "test5 data";

char *test_create()
{
	list = List_create();
	mu_assert(list != NULL, "Failed to create list.");

	return NULL;
}

char *test_destroy()
{
	List_clear_destroy(list);

	return NULL;
}

char *test_push_pop()
{
	List_push(list, test1);
	mu_assert(List_last(list) == test1, "Wrong last value.");

	List_push(list, test2);
	mu_assert(List_last(list) == test2, "Wrong last value.");

	List_push(list, test3);
	mu_assert(List_last(list) == test3, "Wrong last value.");
	mu_assert(List_count(list) == 3, "Wrong count on push.");

	char *val = List_pop(list);
	mu_assert(val == test3, "Wrong value on pop.");

	val = List_pop(list);
	mu_assert(val == test2, "Wrong value on pop.");
	
	val = List_pop(list);
	mu_assert(val == test1, "Wrong value on pop.");
	mu_assert(List_count(list) == 0, "Wrong count after pop.");

	return NULL;
}

char *test_unshift()
{
	List_unshift(list, test1);
	mu_assert(List_first(list) == test1, "Wrong first value.");

	List_unshift(list, test2);
	mu_assert(List_first(list) == test2, "Wrong first value.");

	List_unshift(list, test3);
	mu_assert(List_first(list) == test3, "Wrong last value.");
	mu_assert(List_count(list) == 3, "Wrong count on unshift.");

	return NULL;
}

char *test_remove()
{
	// we only need to test the middle remove case since push/shift
	// already tests the other cases

	char *val = List_remove(list, list->first->next);
	mu_assert(val == test2, "Wrong removed element.");
	mu_assert(List_count(list) == 2, "Wrong count after remove.");
	mu_assert(List_first(list) == test3, "Wrong first after remove.");
	mu_assert(List_last(list) == test1, "Wrong last after remove.");

	return NULL;
}

char *test_shift()
{
	mu_assert(List_count(list) != 0, "Wrong count before shift.")

	char *val = List_shift(list);
	mu_assert(val == test3, "Wrong value on shift.");

	val = List_shift(list);
	mu_assert(val == test1, "Wrong value on shift.");
	mu_assert(List_count(list) == 0, "Wrong count after shift.");

	return NULL;
}

char *test_copy()
{
	List *list1 = List_create();

	List_push(list1, test1);
	List_push(list1, test2);
	List_push(list1, test3);

	List *list1_copy = List_copy(list1);

	List_clear_destroy(list1);
	
	mu_assert(List_first(list1_copy) == test1, "Wrong first value.");
	mu_assert(List_last(list1_copy) == test3, "Wrong last value.");
	mu_assert(List_count(list1_copy) == 3, "Wrong count on copy.");

	List_clear_destroy(list1_copy);

	return NULL;
}

char *test_join()
{
	// first join test
	List *list1 = List_create();

	List_push(list1, test1);

	List *list2 = List_create();

	List_push(list2, test2);
	List_push(list2, test3);

	List *list3 = List_create();
	
	List_push(list3, test4);
	List_push(list3, test5);

	List_join(list1, list2, list3);

	List_clear_destroy(list2);
	List_clear_destroy(list3);

	mu_assert(List_first(list1) == test1, "Wrong first value.");
	mu_assert(List_last(list1) == test5, "Wrong last value.");
	mu_assert(List_count(list1) == 5, "Wrong count on join.");

	List_clear_destroy(list1);
	
	// second join test
	list1 = List_create();
	list2 = List_create();
	list3 = List_create();

	List *list4 = List_create();

	List_push(list4, test4);

	List_join(list1, list2, list4, list3);

	List_clear_destroy(list2);
	List_clear_destroy(list3);
	List_clear_destroy(list4);

	mu_assert(List_first(list1) == test4, "Wrong first value.");
	mu_assert(List_last(list1) == test4, "Wrong last value.");
	mu_assert(List_count(list1) == 1, "Wrong count on join.");

	List_clear_destroy(list1);

	// third join test
	list1 = List_create();
	list2 = List_create();
	list3 = List_create();
	
	List_join(list1, list2, list3);

	List_clear_destroy(list2);
	List_clear_destroy(list3);

	mu_assert(List_count(list1) == 0, "Wrong count on join.");

	List_clear_destroy(list1);

	return NULL;
}

char *test_split()
{
	// first split test
	List *list1 = List_create();

	List_push(list1, test1);
	List_push(list1, test2);
	List_push(list1, test3);
	List_push(list1, test4);
	List_push(list1, test5);

	List *sub_list1 = List_create();
	List *sub_list2 = List_create();
	List *sub_list3 = List_create();

	List_split(list1, sub_list1, 1, sub_list2, 2, sub_list3, 2);

	List_clear_destroy(list1);

	mu_assert(List_first(sub_list1) == test1, "Wrong first value.");
	mu_assert(List_last(sub_list1) == test1, "Wrong last value.");
	mu_assert(List_count(sub_list1) == 1, "Wrong count on split.");

	mu_assert(List_first(sub_list2) == test2, "Wrong first value.");
	mu_assert(List_last(sub_list2) == test3, "Wrong last value.");
	mu_assert(List_count(sub_list2) == 2, "Wrong count on split.");

	mu_assert(List_first(sub_list3) == test4, "Wrong first value.");
	mu_assert(List_last(sub_list3) == test5, "Wrong last value.");
	mu_assert(List_count(sub_list3) == 2, "Wrong count on split.");

	List_clear_destroy(sub_list1);
	List_clear_destroy(sub_list2);
	List_clear_destroy(sub_list3);

	// second split test
	list1 = List_create();

	List_push(list1, test1);
	List_push(list1, test2);
	List_push(list1, test3);

	sub_list1 = List_create();
	sub_list2 = List_create();
	sub_list3 = List_create();

	List_split(list1, sub_list1, 1, sub_list2, 0, sub_list3, 100);

	List_clear_destroy(list1);

	mu_assert(List_count(sub_list1) == 1, "Wrong count on split.");
	mu_assert(List_count(sub_list2) == 0, "Wrong count on split.");
	mu_assert(List_count(sub_list3) == 2, "Wrong count on split.");

	mu_assert(List_first(sub_list3) == test2, "Wrong first value.");
	mu_assert(List_last(sub_list3) == test3, "Wrong last value.");

	List_clear_destroy(sub_list1);
	List_clear_destroy(sub_list2);
	List_clear_destroy(sub_list3);

	return NULL;
}

char *all_tests() {
	mu_suite_start();

	mu_run_test(test_create);
	mu_run_test(test_push_pop);
	mu_run_test(test_unshift);
	mu_run_test(test_remove);
	mu_run_test(test_shift);
	mu_run_test(test_destroy);

	mu_run_test(test_copy);
	mu_run_test(test_join);
	mu_run_test(test_split);

	return NULL;
}

RUN_TESTS(all_tests);
