#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "CTL_container.h"

typedef struct
{
    CTL_container container;
    CTL_functions *functions;
    bool (*compare_handler)(void *a, void *b);
} CTL_priority_queue;

void CTL_priority_queue_new(CTL_priority_queue *handle, bool (*compare_handler)(void *a, void *b), CTL_functions *functions, const size_t buf_size, const size_t T_size);

void CTL_priority_queue_delete(CTL_priority_queue *handle);

void *CTL_priority_queue_top(const CTL_priority_queue *handle);

void CTL_priority_queue_push(CTL_priority_queue *handle, const void *data);

void CTL_priority_queue_pop(CTL_priority_queue *handle);

size_t CTL_priority_queue_size(const CTL_priority_queue *handle);

bool CTL_priority_queue_empty(const CTL_priority_queue *handle);