#include <stddef.h>
#include <stdbool.h>

#include "air/ctl/queue.h"

void CTL_queue_new(CTL_queue *handle, CTL_functions *functions, size_t data_size, size_t T_size)
{
    handle->functions = functions;
    handle->functions->__new(&handle->container, data_size, T_size);
}

void CTL_queue_delete(CTL_queue *handle)
{
    handle->functions->__delete(&handle->container);
}

void *CTL_queue_front(const CTL_queue *handle)
{
    return handle->functions->front(&handle->container);
}

void *CTL_queue_back(const CTL_queue *handle)
{
    return handle->functions->back(&handle->container);
}

void CTL_queue_push(CTL_queue *handle, const void *element)
{
    handle->functions->push_back(&handle->container, element);
}

void CTL_queue_pop(CTL_queue *handle)
{
    handle->functions->pop_front(&handle->container);
}

size_t CTL_queue_size(const CTL_queue *handle)
{
    return handle->functions->size(&handle->container);
}

bool CTL_queue_empty(const CTL_queue *handle)
{
    return handle->functions->empty(&handle->container);
}