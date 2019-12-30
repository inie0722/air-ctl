#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "CTL_container.h"

typedef struct
{
    CTL_container container;
    CTL_functions *functions;

} CTL_queue;

void CTL_queue_new(CTL_queue *handle, CTL_functions *functions, const size_t buf_size, const size_t T_size);

void CTL_queue_delete(CTL_queue *handle);

void *CTL_queue_front(const CTL_queue *handle);

void *CTL_queue_back(const CTL_queue *handle);

void CTL_queue_push(CTL_queue *handle, const void *data);

void CTL_queue_pop(CTL_queue *handle);

size_t CTL_queue_size(const CTL_queue *handle);

bool CTL_queue_empty(const CTL_queue *handle);