#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>
#include <assert.h>

#define SUB_LIST_MIN_SIZE 3

/*
 * You should use "check_sorting" macro.
*/
int is_sorted(List *list)
{
	assert(list != NULL && "list can't be NULL");

	LIST_FOREACH(list, first, next, cur) {
		if(cur->next && strcmp(cur->value, cur->next->value) > 0) {
			debug("%s %s", (char *)cur->value, (char *)cur->next->value);

			return 0;
		}
	}

	return 1;
}

inline void ListNode_swap(ListNode *a, ListNode *b)
{
	void *temp = a->value;
	a->value = b->value;
	b->value = temp;
}

int List_bubble_sort(List *list, List_compare cmp)
{
	assert(list != NULL && "list can't be NULL");
	assert(cmp != NULL && "cmp can't be NULL");

	int n = List_count(list);

	if(n <= 1) {
		return 0; // already sorted
	}

	do {
		int j = 0;
		int new_n = 0;

		LIST_FOREACH(list, first, next, cur) {
			if(j == n - 1) break;
		
			if(cmp(cur->value, cur->next->value) > 0) {
				ListNode_swap(cur, cur->next);

				new_n = j + 1;
			}

			j++;
		}

		n = new_n;

	} while(n != 0);

	return 0;
}

inline List *List_merge(List *left, List *right, List_compare cmp)
{
	List *result = List_create();
	void *val = NULL;

	while(List_count(left) > 0 || List_count(right) > 0) {
		if(List_count(left) > 0 && List_count(right) > 0) {
			if(cmp(List_first(left), List_first(right)) <= 0) {
				val = List_shift(left);
			} else {
				val = List_shift(right);
			}
			List_push(result, val);
		} else if(List_count(left) > 0) {
			List_join(result, left);
		} else if(List_count(right) > 0) {
			List_join(result, right);
		}
	}

	return result;
}

List *List_merge_sort(List *list, List_compare cmp)
{
	assert(list != NULL && "list can't be NULL");
	assert(cmp != NULL && "cmp can't be NULL");

	if(List_count(list) <= SUB_LIST_MIN_SIZE) {
		int rc = List_bubble_sort(list, cmp);

		assert(rc == 0 && "Bubble sort failed.");

		return list;
	}

	List *left = List_create();
	List *right = List_create();

	int middle = List_count(list) / 2;

	List_split(list, left, middle, right, List_count(list) - middle);

	List *sort_left = List_merge_sort(left, cmp);
	List *sort_right = List_merge_sort(right, cmp);

	if(sort_left != left) List_clear_destroy(left);
	if(sort_right != right) List_clear_destroy(right);
	
	List *merged_list = List_merge(sort_left, sort_right, cmp);

	List_clear_destroy(sort_left);
	List_clear_destroy(sort_right);

	return merged_list;
}
