#pragma once

#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    char *data;
    ptrdiff_t T_size;
} CTL_array_iterator;

typedef struct
{
    CTL_array_iterator begin;
    CTL_array_iterator end;
} CTL_array;

void CTL_array_new(CTL_array *handle, const void *begin, const void *end, const size_t T_size);

void *CTL_array_front(const CTL_array *handle);

void *CTL_array_back(const CTL_array *handle);

CTL_array_iterator CTL_array_begin(const CTL_array *handle);

CTL_array_iterator CTL_array_end(const CTL_array *handle);

CTL_array_iterator CTL_array_at(const CTL_array *handle, const size_t pos);

CTL_array_iterator CTL_array_iterator_move(const CTL_array_iterator *handle, const ptrdiff_t pos);

bool CTL_array_iterator_equal(const CTL_array_iterator *left, const CTL_array_iterator *right);

ptrdiff_t CTL_array_iterator_diff(const CTL_array_iterator *left, const CTL_array_iterator *right);

bool CTL_array_iterator_more(const CTL_array_iterator *left, const CTL_array_iterator *right);

size_t CTL_array_size(const CTL_array *handle);

size_t CTL_array_capacity(const CTL_array *handle);
