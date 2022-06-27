#pragma once


#include <stddef.h>
#include <stdbool.h>

#include "CTL/config.h"
#include "CTL/compat/atomic.h"

typedef struct
{
    char *data;
    size_t T_size;
    size_t max_size;
    atomic_size_t writable_limit;
    CTL_CACHE_LINE_ALIGN(atomic_size_t, 0)
    atomic_size_t readable_limit;
} CTL_lockfree_spsc_queue;

#define CTL_SPSC_QUEUE_LOCK_FREE ATOMIC_LONG_LOCK_FREE

CTL_API extern void CTL_lockfree_spsc_queue_new(CTL_lockfree_spsc_queue *handle, size_t max_size, size_t T_size);

CTL_API extern void CTL_lockfree_spsc_queue_delete(CTL_lockfree_spsc_queue *handle);

CTL_API extern size_t CTL_lockfree_spsc_queue_push(CTL_lockfree_spsc_queue *handle, const void *first, size_t count);

CTL_API extern size_t CTL_lockfree_spsc_queue_pop(CTL_lockfree_spsc_queue *handle, void *first, size_t count);

CTL_API extern size_t CTL_lockfree_spsc_queue_size(const CTL_lockfree_spsc_queue *handle);

CTL_API extern bool CTL_lockfree_spsc_queue_empty(const CTL_lockfree_spsc_queue *handle);