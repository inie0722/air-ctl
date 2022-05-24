#include <stddef.h>
#include <stdbool.h>

#include "CTL/priority_queue.h"
#include "CTL/heap.h"

void CTL_priority_queue_new(CTL_priority_queue *handle, bool (*compare_handler)(const void *left, const void *right), CTL_functions *functions, size_t data_size, size_t T_size)
{
    handle->functions = functions;
    handle->compare_handler = compare_handler;
    handle->functions->__new(&handle->container, data_size, T_size);
}

void CTL_priority_queue_delete(CTL_priority_queue *handle)
{
    handle->functions->__delete(&handle->container);
}

void *CTL_priority_queue_top(const CTL_priority_queue *handle)
{
    return handle->functions->front(&handle->container);
}

void CTL_priority_queue_push(CTL_priority_queue *handle, const void *element)
{
    handle->functions->push_back(&handle->container, element);

    CTL_iterator begin;
    CTL_iterator end;
    handle->functions->begin(&handle->container, &begin);
    handle->functions->end(&handle->container, &end);
    CTL_push_heap(handle->functions, &begin, &end, handle->compare_handler);
}

void CTL_priority_queue_pop(CTL_priority_queue *handle)
{
    CTL_iterator begin;
    CTL_iterator end;
    handle->functions->begin(&handle->container, &begin);
    handle->functions->end(&handle->container, &end);
    CTL_pop_heap(handle->functions, &begin, &end, handle->compare_handler);

    handle->functions->pop_back(&handle->container);
}

size_t CTL_priority_queue_size(const CTL_priority_queue *handle)
{
    return handle->functions->size(&handle->container);
}

bool CTL_priority_queue_empty(const CTL_priority_queue *handle)
{
    return handle->functions->empty(&handle->container);
}