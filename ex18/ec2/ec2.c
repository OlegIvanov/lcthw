// Write another sorting algorithm, then change test_sorting so that 
// it takes both an arbitrary sort function and the sort function's callback comparison. 
// Use it to test both of your algorithms.

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/** Our old friend from ex17. */
void die(const char *message)
{
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	exit(1);
}

// a typedef creates a fake type, in this
// case for a function pointer
typedef int (*compare_cb)(int a, int b);

typedef int* (*sort_function)(int*, int, void*);

/**
 * A classic bubble sort function that uses the
 * compare_cb to do the sorting.
 */
int *bubble_sort(int *numbers, int count, void* sort_order_function)
{
	int temp = 0;
	int i = 0;
	int j = 0;
	int *target = malloc(count * sizeof(int));

	if(!target) die("Memory error.");

	memcpy(target, numbers, count * sizeof(int));

	compare_cb cmp = sort_order_function;
	
	for(i = 0; i < count; i++) {
		for(j = 0; j < count - 1; j++) {
			if(cmp(target[j], target[j+1]) > 0) {
				temp = target[j+1];
				target[j+1] = target[j];
				target[j] = temp;
			}
		}
	}
	
	return target;
}

int sorted_order(int a, int b)
{
	return a - b;
}

int reverse_order(int a, int b)
{
	return b - a;
}

int strange_order(int a, int b)
{
	if(a == 0 || b == 0) {
		return 0;
	} else {
		return a % b;
	}
}

/** 
 * Used to test that we are sorting things correctly
 * by doing the sort and printing it out.
 */
void test_sorting(int *numbers, int count, sort_function sf, void* sort_order_function)
{
	int i = 0;

	int *sorted = sf(numbers, count, sort_order_function);

	if(!sorted) die("Failed to sort as requested.");

	for(i = 0; i < count; i++) {
		printf("%d ", sorted[i]);
	}
	printf("\n");

	free(sorted);
}

int main(int argc, char *argv[])
{
	if(argc < 2) die("USAGE: ex18 4 3 1 5 6");
	
	int count = argc - 1;
	int i = 0;
	char **inputs = argv + 1;

	int *numbers = malloc(count * sizeof(int));
	if(!numbers) die("Memory error.");

	for(i = 0; i < count; i++) {
		numbers[i] = atoi(inputs[i]);
	}

	test_sorting(numbers, count, bubble_sort, sorted_order);
	test_sorting(numbers, count, bubble_sort, reverse_order);
	test_sorting(numbers, count, bubble_sort, strange_order);

	free(numbers);

	return 0;
}
