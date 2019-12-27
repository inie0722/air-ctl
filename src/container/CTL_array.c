#include <malloc.h>
#include <string.h>
#include <stdbool.h>

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
    return CTL_array_iterator_move(&handle->end, -1).data;
}

CTL_array_iterator CTL_array_begin(const CTL_array *handle)
{
    return handle->begin;
}

CTL_array_iterator CTL_array_end(const CTL_array *handle)
{
    return handle->end;
}

CTL_array_iterator CTL_array_at(const CTL_array *handle, const size_t pos)
{
    CTL_array_iterator result;
    result.T_size = handle->end.T_size;
    result.data = handle->begin.data + (pos * handle->end.T_size);
    return result;
}

CTL_array_iterator CTL_array_iterator_move(const CTL_array_iterator *handle, const ptrdiff_t pos)
{
    CTL_array_iterator result;
    result.T_size = handle->T_size;
    result.data = handle->data + (pos * handle->T_size);
    return result;
}

bool CTL_array_iterator_equal(const CTL_array_iterator *left, const CTL_array_iterator *right)
{
    return left->data == right->data;
}

ptrdiff_t CTL_array_iterator_diff(const CTL_array_iterator *left, const CTL_array_iterator *right)
{
    return (left->data - right->data) / left->T_size;
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