#include <lcthw/list.h>
#include <lcthw/dbg.h>
#include <assert.h>
#include <unistd.h>
#include <stdarg.h>

#define BUF_SIZE 1000

// Warning! Not portable!
void Heap_get_range(void **heap_low, void **heap_high)
{
	char maps_filename[BUF_SIZE];
	char file_line[BUF_SIZE];
	const char *heap = "[heap]";

	pid_t pid = getpid();

	sprintf(maps_filename, "/proc/%d/maps", pid);
	
	FILE *maps_file = fopen(maps_filename, "r");
	check(maps_file, "maps file has't been opened");

	while(fgets(file_line, BUF_SIZE, maps_file) != NULL) {
		if(strstr(file_line, heap) != NULL) {
			break;
		}
	}
		
	sscanf(file_line, "%p-%p", heap_low, heap_high);

	fclose(maps_file);

error:
	return;
}

List *List_create()
{
	return calloc(1, sizeof(List));
}

void List_clear_destroy(List *list)
{
	assert(list != NULL && "list can't be NULL");

	void *heap_low = NULL;
	void *heap_high = NULL;

	Heap_get_range(&heap_low, &heap_high);
	
	LIST_FOREACH(list, first, next, cur) {
		if(cur->value >= heap_low && cur->value <= heap_high) {
			free(cur->value);
		}
		if(cur->prev) {
			free(cur->prev);
		}
	}

	free(list->last);
	free(list);
}

void List_push(List *list, void *value)
{
	assert(list != NULL && "list can't be NULL");

	ListNode *node = calloc(1, sizeof(ListNode));
	check_mem(node);

	node->value = value;

	if(list->last == NULL) {
		list->first = node;
		list->last = node;
	} else {
		list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}

	list->count++;

error:
	return;
}

void *List_pop(List *list)
{
	assert(list != NULL && "list can't be NULL");

	ListNode *node = list->last;
	return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List *list, void *value)
{
	assert(list != NULL && "list can't be NULL");

	ListNode *node = calloc(1, sizeof(ListNode));
	check_mem(node);

	node->value = value;

	if(list->first == NULL) {
		list->first = node;
		list->last = node;
	} else {
		node->next = list->first;
		list->first->prev = node;
		list->first = node;
	}

	list->count++;

error:
	return;
}

void *List_shift(List *list)
{
	assert(list != NULL && "list can't be NULL");

	ListNode *node = list->first;
	return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List *list, ListNode *node)
{
	assert(list != NULL && "list can't be NULL");

	void *result = NULL;

	check(list->first && list->last, "List is empty.");
	check(node, "node can't be NULL");

	if(node == list->first && node == list->last) {
		list->first = NULL;
		list->last = NULL;
	} else if(node == list->first) {
		list->first = node->next;
		check(list->first != NULL, "Invalid list, somehow got a first that is NULL.");
		list->first->prev = NULL;
	} else if(node == list->last) {
		list->last = node->prev;
		check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
		list->last->next = NULL;
	} else {
		ListNode *after = node->next;
		ListNode *before = node->prev;
		after->prev = before;
		before->next = after;
	}

	list->count--;
	result = node->value;
	free(node);

error:
	return result;
}

List *List_copy(List *list)
{
	check(list, "list can't be NULL");
	
	List *list_copy = calloc(1, sizeof(List));
	check_mem(list_copy);

	LIST_FOREACH(list, first, next, cur) {
		List_push(list_copy, cur->value);
	}

	return list_copy;

error:
	return NULL;
}

/* 
 * You should use "List_join" macro.
*/
int List_join_hidden(List *list, ...)
{
	check(list, "list_start can't be NULL")

	va_list	argp;
	va_start(argp, list);

	List *sub_list = NULL;

	for(sub_list = va_arg(argp, List *);
		sub_list != NULL;
		sub_list = va_arg(argp, List *)) {
		
		// if we have no elements in the sub_list
		if(!sub_list->last) continue;
		
		// if we have no elements in the list
		if(!list->last) {
			list->first = sub_list->first;
		} else {
			list->last->next = sub_list->first;
			sub_list->first->prev = list->last;
		}

		list->last = sub_list->last;

		list->count += sub_list->count;
		
		// preparing for succesfull destruction
		sub_list->first = NULL;
		sub_list->last = NULL;
	}

	va_end(argp);

	return 0;

error:
	return -1;
}

/*
 * You should use "List_split" macro.
*/
int List_split_hidden(List *list, ...)
{
	check(list, "list can't be NULL");

	va_list	argp;
	va_start(argp, list);

	int sub_list_count = 0;
	List *sub_list = NULL;

	LIST_FOREACH(list, first, next, cur) {
		while(sub_list_count == 0) {
			sub_list = va_arg(argp, List *);

			if(!sub_list) break;

			sub_list_count = va_arg(argp, int);

			check(sub_list_count >= 0, "sub_list_count can't be negative");
		}

		if(!sub_list) break;

		List_push(sub_list, cur->value);

		sub_list_count--;
	}

	va_end(argp);

	return 0;

error:
	return -1;
}
