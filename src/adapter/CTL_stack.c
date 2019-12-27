#include <stddef.h>
#include <stdbool.h>

#include "CTL_stack.h"

void CTL_stack_new(CTL_stack *handle, CTL_functions *functions, size_t buf_size, size_t T_size)
{
    handle->functions = functions;
    handle->functions->__new(&handle->container, buf_size, T_size);
}

void CTL_stack_delete(CTL_stack *handle)
{
    handle->functions->__delete(&handle->container);
}

void *CTL_stack_top(const CTL_stack *handle)
{
    return handle->functions->back(&handle->container);
}

void CTL_stack_push(CTL_stack *handle, void* data)
{
    handle->functions->push_back(&handle->container, data);
}

void CTL_stack_pop(CTL_stack *handle)
{
    handle->functions->pop_back(&handle->container);
}

size_t CTL_stack_size(const CTL_stack *handle)
{
    return handle->functions->size(&handle->container);
}

bool CTL_stack_empty(const CTL_stack *handle)
{
    return handle->functions->empty(&handle->container);
}