#pragma once

#include <stdbool.h>
#include <stddef.h>

/*声明*/
typedef struct __CTL_list_node
{
	char *data;
	struct __CTL_list_node *prior;
	struct __CTL_list_node *next;
} __CTL_list_node;

typedef struct
{
	char *data;
	long T_size;
	__CTL_list_node *node;
} CTL_list_iterator;

typedef struct
{
	long T_size;
	__CTL_list_node *list;
} CTL_list;

extern void CTL_list_new(CTL_list *handle, const size_t T_size);

extern void CTL_list_clear(CTL_list *handle);

extern void CTL_list_delete(CTL_list *handle);

extern void *CTL_list_front(const CTL_list *handle);

extern void *CTL_list_back(const CTL_list *handle);

extern void CTL_list_begin(const CTL_list *handle, CTL_list_iterator *ret);

extern void CTL_list_end(const CTL_list *handle, CTL_list_iterator *ret);

extern void CTL_list_push_back(CTL_list *handle, const void *data);

extern void CTL_list_push_front(CTL_list *handle, const void *data);

extern void CTL_list_pop_back(CTL_list *handle);

extern void CTL_list_pop_front(CTL_list *handle);

extern void CTL_list_insert(CTL_list *handle, CTL_list_iterator *iterator, const void *data);

extern void CTL_list_erase(CTL_list *handle, CTL_list_iterator *iterator);

extern void CTL_list_splice(CTL_list *handle, const CTL_list_iterator *position, const CTL_list_iterator *first, const CTL_list_iterator *last);

extern void CTL_list_merge(CTL_list *handle, CTL_list *x, bool (*compare_handler)(void *a, void *b));

extern void CTL_list_sort(CTL_list *handle, bool (*compare_handler)(void *a, void *b));

extern void CTL_list_iterator_move(const CTL_list_iterator *handle, const ptrdiff_t index, CTL_list_iterator *ret);

extern bool CTL_list_iterator_equal(const CTL_list_iterator *left, const CTL_list_iterator *right);

extern size_t CTL_list_size(const CTL_list *handle);

extern bool CTL_list_empty(const CTL_list *handle);
