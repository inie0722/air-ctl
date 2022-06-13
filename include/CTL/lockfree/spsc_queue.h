#pragma once

#include <stdatomic.h>
#include <stddef.h>

#include "CTL/config.h"

typedef struct  
{
    char *data;
    size_t T_size;
    size_t max_size;
    atomic_size_t writable_limit;
    atomic_size_t readable_limit;
}CTL_lockfree_spsc_queue;

CTL_API extern void CTL_lockfree_spsc_queue_new(CTL_lockfree_spsc_queue *handle, size_t max_size, size_t T_size);

CTL_API extern void CTL_lockfree_spsc_queue_delete(CTL_lockfree_spsc_queue *handle);