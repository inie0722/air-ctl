#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "CTL_container.h"
#include "CTL_allocator.h"

typedef struct
{
    CTL_container container;
    CTL_functions *functions;

}CTL_stack;

void CTL_stack_new(CTL_stack *handle, CTL_functions *functions, size_t buf_size, size_t T_size);

void CTL_stack_delete(CTL_stack *handle);

void *CTL_stack_top(const CTL_stack *handle);

void CTL_stack_push(CTL_stack *handle, void *data);

void CTL_stack_pop(CTL_stack *handle);

size_t CTL_stack_size(const CTL_stack *handle);

bool CTL_stack_empty(const CTL_stack *handle);