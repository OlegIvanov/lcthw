#include <lcthw/darray.h>
#include <lcthw/darray_algos.h>
#include <assert.h>

DArray *DArray_create(size_t element_size, size_t initial_max)
{
	DArray *array = malloc(sizeof(DArray));
	check_mem(array);

	array->max = initial_max;
	check(array->max > 0, "You must set an initial_max > 0.");
	
	array->contents = calloc(initial_max, sizeof(void *));
	check_mem(array->contents);

	array->end = 0;

	array->element_size = element_size;
	check(array->element_size > 0, "You must set an element_size > 0.");

	array->expand_rate = DEFAULT_EXPAND_RATE;

	return array;

error:
	if(array) free(array);
	return NULL;
}

int DArray_clear(DArray *array)
{
	check(array, "array can't be NULL");

	int i = 0;
	if(array->element_size > 0) {
		for(i = 0; i < array->max; i++) {
			if(array->contents[i] != NULL) {
				free(array->contents[i]);
			}
		}
	}

	return 0;
error:
	return -1;
}

static inline int DArray_resize(DArray *array, size_t newsize)
{
	check(array, "array can't be NULL");

	array->max = newsize;
	check(array->max > 0, "The new size must be > 0.");

	void *contents = realloc(array->contents, array->max * sizeof(void *));
	// check contents and assume realloc doesn't harm the original on error
	check_mem(contents);
	
	array->contents = contents;

	return 0;
error:
	return -1;
}

int DArray_expand(DArray *array)
{
	check(array, "array can't be NULL");

	size_t old_max = array->max;
	check(DArray_resize(array, array->max + array->expand_rate) == 0,
			"Failed to expand array to new size: %d",
			array->max + (int)array->expand_rate);

	memset(array->contents + old_max, 0, array->expand_rate * sizeof(void *));
	return 0;
error:
	return -1;
}

int DArray_contract(DArray *array)
{
	check(array, "array can't be NULL");

	int new_size = array->end < (int)array->expand_rate ? (int)array->expand_rate : array->end;
	
	return DArray_resize(array, new_size + 1);
error:
	return -1;
}

void DArray_destroy(DArray *array)
{
	if(array) {
		if(array->contents) free(array->contents);
		free(array);
	}
}

void DArray_clear_destroy(DArray *array)
{
	DArray_clear(array);
	DArray_destroy(array);
}

int DArray_push(DArray *array, void *el)
{
	check(array, "array can't be NULL");

	array->contents[array->end] = el;
	array->end++;

	if(DArray_end(array) >= DArray_max(array)) {
		return DArray_expand(array);
	} else {
		return 0;
	}
error:
	return -1;
}

void *DArray_pop(DArray *array)
{
	check(array, "array can't be NULL");
	check(array->end - 1 >= 0, "Attempt to pop from empty array.");
	
	void *el = DArray_remove(array, array->end - 1);
	array->end--;

	if(DArray_end(array) > (int)array->expand_rate && DArray_end(array) % array->expand_rate) {
		DArray_contract(array);
	}

	return el;
error:
	return NULL;
}

int DArray_sort_add(DArray *array, void *el, DArray_compare cmp)
{
	check(array, "array can't be NULL");

	int rc = DArray_push(array, el);
	check(rc == 0, "push failed");

	rc = DArray_quicksort(array, cmp);
	check(rc == 0, "sort failed");

	return 0;
error:
	return -1;
}

int DArray_find(DArray *array, void *to_find, DArray_compare cmp)
{
	check(array, "array can't be NULL");

	int low = 0;
	int high = DArray_end(array) - 1;

	int middle = 0;
	void *value = NULL;
	int cmp_result = 0;

	while(low <= high) {
		middle = low + (high - low) / 2;
		value = DArray_get(array, middle);

		cmp_result = cmp(&to_find, &value);

		if(cmp_result < 0) {
			high = middle - 1;
		} else if(cmp_result > 0) {
			low = middle + 1;
		} else {
			return middle;
		}
	}
	
	return -1;
error:
	return -2;
}
