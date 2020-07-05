#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "CTL_array.h"

void CTL_array_init(CTL_array *handle, const void *begin, const void *end, const size_t T_size)
{
    handle->begin.data = (char *)begin;
    handle->begin.T_size = T_size;

    handle->end.data = (char *)end;
    handle->end.T_size = T_size;
}

void *CTL_array_front(const CTL_array *handle)
{
    return handle->begin.data;
}

void *CTL_array_back(const CTL_array *handle)
{
    return handle->end.data - handle->end.T_size;
}

void CTL_array_begin(const CTL_array *handle, CTL_array_iterator *ret)
{
    *ret = handle->begin;
}

void CTL_array_end(const CTL_array *handle, CTL_array_iterator *ret)
{
    *ret = handle->end;
}

void *CTL_array_at(const CTL_array *handle, const size_t index)
{
    return handle->begin.data + (index * handle->end.T_size);
}

void CTL_array_iterator_move(const CTL_array_iterator *handle, const ptrdiff_t index, CTL_array_iterator *ret)
{
    ret->T_size = handle->T_size;
    ret->data = handle->data + (index * handle->T_size);
}

bool CTL_array_iterator_equal(const CTL_array_iterator *left, const CTL_array_iterator *right)
{
    return left->data == right->data;
}

ptrdiff_t CTL_array_iterator_diff(const CTL_array_iterator *left, const CTL_array_iterator *right)
{
    return (left->data - right->data) / (ptrdiff_t)left->T_size;
}

bool CTL_array_iterator_more(const CTL_array_iterator *left, const CTL_array_iterator *right)
{
    return left->data > right->data;
}

size_t CTL_array_size(const CTL_array *handle)
{
    return CTL_array_iterator_diff(&handle->end, &handle->begin);
}

size_t CTL_array_capacity(const CTL_array *handle)
{
    return CTL_array_iterator_diff(&handle->end, &handle->begin);
}

bool CTL_array_empty(const CTL_array *handle)
{
    return !handle->begin.data;
}