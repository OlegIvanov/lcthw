#ifndef lcthw_List_algos_h
#define lcthw_List_algos_h

#include <lcthw/list.h>

typedef int (*List_compare)(const void *a, const void *b);

int List_bubble_sort(List *list, List_compare cmp);

List *List_merge_sort(List *list, List_compare cmp);

int is_sorted(List *list, List_compare cmp);

#ifdef NDEBUG
#define check_sorting(list, cmp)
#else
#define check_sorting(list, cmp) \
	is_sorted(list, cmp)
#endif

#endif
