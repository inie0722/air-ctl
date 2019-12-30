#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "CTL_container.h"

typedef struct
{
    CTL_container container;
    CTL_functions *functions;

} CTL_stack;

extern void CTL_stack_new(CTL_stack *handle, CTL_functions *functions, const size_t buf_size, const size_t T_size);

extern void CTL_stack_delete(CTL_stack *handle);

extern void *CTL_stack_top(const CTL_stack *handle);

extern void CTL_stack_push(CTL_stack *handle, const void *data);

extern void CTL_stack_pop(CTL_stack *handle);

extern size_t CTL_stack_size(const CTL_stack *handle);

extern bool CTL_stack_empty(const CTL_stack *handle);