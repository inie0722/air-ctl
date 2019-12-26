#pragma once

#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>

#include "CTL_allocator.h"

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
	__CTL_list_node *node;
	size_t T_size;
} CTL_list_iterator;

typedef struct
{
	__CTL_list_node *list;
	size_t T_size;
} CTL_list;

void CTL_list_new(CTL_list *handle, const size_t T_size);

void CTL_list_clear(CTL_list *handle);

void CTL_list_delete(CTL_list *handle);

void *CTL_list_front(const CTL_list *handle);

void *CTL_list_back(const CTL_list *handle);

CTL_list_iterator CTL_list_begin(const CTL_list *handle);

CTL_list_iterator CTL_list_end(const CTL_list *handle);

void CTL_list_push_back(CTL_list *handle, const void *data);

void CTL_list_push_front(CTL_list *handle, const void *data);

void CTL_list_pop_back(CTL_list *handle);

void CTL_list_pop_front(CTL_list *handle);

void CTL_list_insert(CTL_list *handle, const CTL_list_iterator *iterator, const void *data);

void CTL_list_erase(CTL_list *handle, const CTL_list_iterator *iterator);

void CTL_list_splice(CTL_list *handle, const CTL_list_iterator *position, const CTL_list_iterator *first, const CTL_list_iterator *last);

void CTL_list_merge(CTL_list *handle, CTL_list *x, bool (*compare_handler)(void *a, void *b));

void CTL_list_sort(CTL_list *handle, bool (*compare_handler)(void *a, void *b));

CTL_list_iterator CTL_list_iterator_move(const CTL_list_iterator *handle, const ptrdiff_t pos);

bool CTL_list_iterator_equal(const CTL_list_iterator *left, const CTL_list_iterator *right);

size_t CTL_list_size(const CTL_list *handle);

bool CTL_list_empty(const CTL_list *handle);