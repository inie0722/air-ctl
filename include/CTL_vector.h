#pragma once

#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>

#include "CTL_allocator.h"

typedef struct
{
    char *data;
    ptrdiff_t T_size;
} CTL_vector_iterator;

typedef struct
{
    CTL_vector_iterator begin;
    CTL_vector_iterator end;
    CTL_vector_iterator end_of_storage;
} CTL_vector;

void CTL_vector_new(CTL_vector *handle, const size_t size, const size_t T_size);

void CTL_vector_clear(CTL_vector *handle);

void CTL_vector_delete(CTL_vector *handle);

void *CTL_vector_front(const CTL_vector *handle);

void *CTL_vector_back(const CTL_vector *handle);

CTL_vector_iterator CTL_vector_begin(const CTL_vector *handle);

CTL_vector_iterator CTL_vector_end(const CTL_vector *handle);

void CTL_vector_push_back(CTL_vector *handle, const void *data);

void CTL_vector_pop_back(CTL_vector *handle);

void CTL_vector_insert(CTL_vector *handle, const CTL_vector_iterator *iterator, const void *data);

void CTL_vector_erase(CTL_vector *handle, const CTL_vector_iterator *iterator);

CTL_vector_iterator CTL_vector_at(const CTL_vector *handle, const size_t pos);

CTL_vector_iterator CTL_vector_iterator_move(const CTL_vector_iterator *handle, const ptrdiff_t pos);

bool CTL_vector_iterator_equal(const CTL_vector_iterator *left, const CTL_vector_iterator *right);

ptrdiff_t CTL_vector_iterator_diff(const CTL_vector_iterator *left, const CTL_vector_iterator *right);

bool CTL_vector_iterator_more(const CTL_vector_iterator *left, const CTL_vector_iterator *right);

size_t CTL_vector_size(const CTL_vector *handle);

size_t CTL_vector_capacity(const CTL_vector *handle);