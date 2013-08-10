#include <lcthw/darray_algos.h>
#include <stdlib.h>
#include <lcthw/dbg.h>

int DArray_qsort(DArray *array, DArray_compare cmp)
{
	qsort(array->contents, DArray_count(array), sizeof(void *), cmp);
	return 0;
}

static inline int get_parent_index(int child_index)
{
	check(child_index > 0, "'child_index' can't be '0' because it's root index");

	return (child_index % 2) ? (child_index - 1) / 2 : (child_index - 2) / 2;
error:
	return -1;
}

int DArray_heapsort(DArray *array, DArray_compare cmp)
{
	check(array, "argument 'array' can't be NULL");

	if(DArray_count(array) <= 1) {
		return 0;
	}

	int i = 0, j = 0, current_index = 0, parent_index = 0;
	void *current_value = NULL, *parent_value = NULL, *buf_value = NULL;

	for(i = 0; i < DArray_count(array) - 1; i++) {

		for(j = 1; j < DArray_count(array) - i; j++) {

			current_index = j;
			parent_index = get_parent_index(current_index);

			current_value = DArray_get(array, current_index);
			parent_value = DArray_get(array, parent_index);

			while(cmp(&current_value, &parent_value) > 0) {

				DArray_set(array, current_index, parent_value);
				DArray_set(array, parent_index, current_value);

				// if "root" has been reached
				if(parent_index == 0) break;

				current_index = parent_index;
				parent_index = get_parent_index(current_index);

				current_value = DArray_get(array, current_index);
				parent_value = DArray_get(array, parent_index);
			}
		}

		buf_value = DArray_first(array);
		DArray_set(array, 0, DArray_get(array, j - 1));
		DArray_set(array, j - 1, buf_value);
	}

	return 0;
error:
	return -1;
}

static inline int DArray_merge(DArray *array, int start_left_index, int left_count, int right_count, DArray_compare cmp)
{
	check(array, "argument 'array' can't be NULL");

	DArray *merged = DArray_create(1, left_count + right_count);

	int i = 0, left_first_index = 0, right_first_index = 0, left_last_index = 0, right_last_index = 0;

	void *left_first_value = NULL, *right_first_value = NULL;

	left_first_index = start_left_index;
	left_last_index = start_left_index + left_count - 1;

	right_first_index = start_left_index + left_count;
	right_last_index = start_left_index + left_count + right_count - 1;

	for(i = 0; i < left_count + right_count; i++) {

		if(left_first_index <= left_last_index && right_first_index <= right_last_index) {

			left_first_value = DArray_get(array, left_first_index);
			right_first_value = DArray_get(array, right_first_index);

			if(cmp(&left_first_value, &right_first_value) <= 0) {

				DArray_push(merged, left_first_value);
				left_first_index++;

			} else {

				DArray_push(merged, right_first_value);
				right_first_index++;
			}

		} else if(left_first_index <= left_last_index) {

			left_first_value = DArray_get(array, left_first_index);
			DArray_push(merged, left_first_value);
			left_first_index++;

		} else if(right_first_index <= right_last_index) {
			
			right_first_value = DArray_get(array, right_first_index);
			DArray_push(merged, right_first_value);
			right_first_index++;
		}
	}

	for(i = 0; i < left_count + right_count; i++) {
		DArray_set(array, start_left_index + i, DArray_get(merged, i));
	}

	DArray_destroy(merged);
	
	return 0;
error:
	return -1;
}

static int DArray_mergesort_utility(DArray *array, int start_left_index, int count, DArray_compare cmp)
{
	check(array, "argument 'array' can't be NULL");

	if(count <= 1) return 0;

	int left_count = count / 2;
	int right_count = count - left_count;

	int start_right_index = start_left_index + left_count;

	DArray_mergesort_utility(array, start_left_index, left_count, cmp);
	DArray_mergesort_utility(array, start_right_index, right_count, cmp);

	DArray_merge(array, start_left_index, left_count, right_count, cmp);

	return 0;
error:
	return -1;
}

int DArray_mergesort(DArray *array, DArray_compare cmp)
{
	return DArray_mergesort_utility(array, 0, DArray_count(array), cmp);
}
