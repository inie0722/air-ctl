#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "CTL_container.h"

typedef struct
{
    CTL_container container;
    CTL_functions *functions;
} CTL_queue;

extern void CTL_queue_new(CTL_queue *handle, CTL_functions *functions, const size_t buf_size, const size_t T_size);

extern void CTL_queue_delete(CTL_queue *handle);

extern void *CTL_queue_front(const CTL_queue *handle);

extern void *CTL_queue_back(const CTL_queue *handle);

extern void CTL_queue_push(CTL_queue *handle, const void *data);

extern void CTL_queue_pop(CTL_queue *handle);

extern size_t CTL_queue_size(const CTL_queue *handle);

extern bool CTL_queue_empty(const CTL_queue *handle);