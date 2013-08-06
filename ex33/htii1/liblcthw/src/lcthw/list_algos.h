#ifndef lcthw_List_algos_h
#define lcthw_List_algos_h

#include <lcthw/list.h>

typedef int (*List_compare)(const void *a, const void *b);

int List_bubble_sort(List *list, List_compare cmp);

List *List_merge_sort(List *list, List_compare cmp);

int check_sorting_hidden(List *list);

#ifdef NDEBUG
#define check_sorting(list)
#else
#define check_sorting(list) \
	check_sorting_hidden(list)
#endif

#endif
