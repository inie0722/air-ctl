#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    char *data;
    size_t T_size;
} CTL_array_iterator;

typedef struct
{
    CTL_array_iterator begin;
    CTL_array_iterator end;
} CTL_array;

extern void CTL_array_init(CTL_array *handle, const void *begin, const void *end, const size_t T_size);

extern void *CTL_array_front(const CTL_array *handle);

extern void *CTL_array_back(const CTL_array *handle);

extern void CTL_array_begin(const CTL_array *handle, CTL_array_iterator *ret);

extern void CTL_array_end(const CTL_array *handle, CTL_array_iterator *ret);

extern void *CTL_array_at(const CTL_array *handle, const size_t index);

extern void CTL_array_iterator_move(const CTL_array_iterator *handle, const ptrdiff_t index, CTL_array_iterator *ret);

extern bool CTL_array_iterator_equal(const CTL_array_iterator *left, const CTL_array_iterator *right);

extern ptrdiff_t CTL_array_iterator_diff(const CTL_array_iterator *left, const CTL_array_iterator *right);

extern bool CTL_array_iterator_more(const CTL_array_iterator *left, const CTL_array_iterator *right);

extern size_t CTL_array_size(const CTL_array *handle);

extern size_t CTL_array_capacity(const CTL_array *handle);

extern bool CTL_array_empty(const CTL_array *handle);
