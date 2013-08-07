#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>
#include <assert.h>

/*
 * You should use "check_sorting" macro.
*/
int is_sorted(List *list, List_compare cmp)
{
	assert(list != NULL && "list can't be NULL");

	LIST_FOREACH(list, first, next, cur) {
		if(cur->next && cmp(cur->value, cur->next->value) > 0) {
			debug("%s %s", (char *)cur->value, (char *)cur->next->value);

			return 0;
		}
	}

	return 1;
}

inline void ListNode_swap(ListNode *a, ListNode *b)
{
	assert(a != NULL && "a can't be NULL");
	assert(b != NULL && "b can't be NULL");

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

List *List_merge_sort(List *list, List_compare cmp, int sublist_min_size)
{
	assert(list != NULL && "list can't be NULL");
	assert(cmp != NULL && "cmp can't be NULL");

	if(List_count(list) <= sublist_min_size) {
		int rc = List_bubble_sort(list, cmp);

		assert(rc == 0 && "Bubble sort failed.");

		return list;
	}

	List *left = List_create();
	List *right = List_create();

	int middle = List_count(list) / 2;

	List_split(list, left, middle, right, List_count(list) - middle);

	List *sort_left = List_merge_sort(left, cmp, sublist_min_size);
	List *sort_right = List_merge_sort(right, cmp, sublist_min_size);
	/*
	if(sort_left != left) List_clear_destroy(left);
	if(sort_right != right) List_clear_destroy(right);
	*/
	List *merged_list = List_merge(sort_left, sort_right, cmp);
	
	/*
	List_clear_destroy(sort_left);
	List_clear_destroy(sort_right);
	*/

	return merged_list;
}

List *List_insert_sorted(List *list, List_compare cmp)
{
	assert(list != NULL && "list can't be NULL");
	assert(cmp != NULL && "cmp can't be NULL");

	if(List_count(list) <= 1) {
		return list;
	}

	List *sorted_list = List_create();
		
	LIST_FOREACH(list, first, next, cur) {
		ListNode *cur_node = NULL;
		ListNode *insert_before_node = NULL;

		for(cur_node = sorted_list->first;
			cur_node != NULL;
			cur_node = cur_node->next) {

			if(cmp(cur->value, cur_node->value) <= 0) {
				insert_before_node = cur_node;
				break;
			}
		}

		if(insert_before_node == NULL) {
			List_push(sorted_list, cur->value);
		} else if(insert_before_node->prev == NULL) {
			List_unshift(sorted_list, cur->value);
		} else {
			ListNode *new_node = calloc(1, sizeof(ListNode));

			new_node->value = cur->value;
			new_node->next = insert_before_node;
			new_node->prev = insert_before_node->prev;

			insert_before_node->prev->next = new_node;
			insert_before_node->prev = new_node;

			sorted_list->count++;
		}
	}

	return sorted_list;
}

List *List_bottom_up_sort(List *list, List_compare cmp)
{
	assert(list != NULL && "list can't be NULL");
	assert(cmp != NULL && "cmp can't be NULL");

	if(List_count(list) <= 1) {
		return list;
	}
	
	List *list_copy = List_copy(list);

	int run_size = 0;
	int list_size = List_count(list_copy);

	List *sorted_list = NULL;

	List *dropped_list = NULL;
	List *left_list = NULL;
	List *right_list = NULL;
	List *merged_list = NULL;

	int i = 0;

	for(run_size = 1; run_size < list_size; run_size *= 2) {

		sorted_list = List_create();
		
		for(i = 0; i < list_size / (2 * run_size); i++) {

			dropped_list = List_create();
			left_list = List_create();
			right_list = List_create();

			List_split(list_copy, dropped_list, i * (2 * run_size), left_list, run_size, right_list, run_size);
				
			merged_list = List_merge(left_list, right_list, cmp);

			List_join(sorted_list, merged_list);
		}

		if(list_size % (2 * run_size) == 1) {
	
			List_push(sorted_list, list_copy->last->value);
		}

		list_copy = sorted_list;
	}

	return sorted_list;
}
