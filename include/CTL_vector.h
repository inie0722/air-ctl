#pragma once

#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    char *data;
    size_t T_size;
} CTL_vector_iterator;

typedef struct
{
    CTL_vector_iterator begin;
    CTL_vector_iterator end;
    CTL_vector_iterator end_of_storage;
} CTL_vector;

extern void CTL_vector_new(CTL_vector *handle, const size_t buf_size, const size_t T_size);

extern void CTL_vector_clear(CTL_vector *handle);

extern void CTL_vector_delete(CTL_vector *handle);

extern void *CTL_vector_front(const CTL_vector *handle);

extern void *CTL_vector_back(const CTL_vector *handle);

extern void CTL_vector_begin(const CTL_vector *handle, CTL_vector_iterator *ret);

extern void CTL_vector_end(const CTL_vector *handle, CTL_vector_iterator *ret);

extern void CTL_vector_push_back(CTL_vector *handle, const void *data);

extern void CTL_vector_pop_back(CTL_vector *handle);

extern void CTL_vector_insert(CTL_vector *handle, CTL_vector_iterator *iterator, const void *data);

extern void CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator *iterator);

extern void *CTL_vector_at(const CTL_vector *handle, const size_t index);

extern void CTL_vector_iterator_move(const CTL_vector_iterator *handle, const ptrdiff_t index, CTL_vector_iterator *ret);

extern bool CTL_vector_iterator_equal(const CTL_vector_iterator *left, const CTL_vector_iterator *right);

extern ptrdiff_t CTL_vector_iterator_diff(const CTL_vector_iterator *left, const CTL_vector_iterator *right);

extern bool CTL_vector_iterator_more(const CTL_vector_iterator *left, const CTL_vector_iterator *right);

extern size_t CTL_vector_size(const CTL_vector *handle);

extern size_t CTL_vector_capacity(const CTL_vector *handle);

extern bool CTL_vector_empty(const CTL_vector *handle);