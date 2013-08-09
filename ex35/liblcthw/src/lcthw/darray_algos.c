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
	return (child_index % 2) ? (child_index - 1) / 2 : (child_index - 2) / 2;
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

			current_value = DArray_get(current_index);
			parent_value = DArray_get(parent_index);

			while(cmp(current_value, parent_value) > 0) {

				DArray_set(array, current_index, parent_value);
				DArray_set(array, parent_index, current_value);

				current_index = parent_index;
				parent_index = get_parent_index(current_index);

				current_value = DArray_get(current_index);
				parent_value = DArray_get(parent_index);
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
/*
int DArray_mergesort(DArray *array, DArray_compare cmp)
{
	return mergesort(array->contents, DArray_count(array), sizeof(void *), cmp);
}
*/
