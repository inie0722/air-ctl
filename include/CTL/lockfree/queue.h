#pragma once

#include <stddef.h>
#include <stdalign.h>
#include <stdbool.h>

#include "CTL/lockfree/aba_pointer.h"
#include "CTL/lockfree/allocator.h"

typedef struct
{
    alignas(CTL_CACHE_LINE_SIZE) _Atomic CTL_aba_pointer next;
    char data[];
} __CTL_lockfree_queue_node;

typedef struct
{
    size_t T_size;
    CTL_lockfree_allocator alloc;
    alignas(CTL_CACHE_LINE_SIZE) _Atomic CTL_aba_pointer head;
    alignas(CTL_CACHE_LINE_SIZE) _Atomic CTL_aba_pointer tail;
    alignas(CTL_CACHE_LINE_SIZE) atomic_size_t size;
} CTL_lockfree_queue;

#define CTL_ALLOCATOR_LOCK_FREE CTL_APA_POINTER_LOCK_FREE

CTL_API extern void CTL_lockfree_queue_new(CTL_lockfree_queue *handle, size_t T_size);

CTL_API extern void CTL_lockfree_queue_delete(CTL_lockfree_queue *handle);

CTL_API extern void CTL_lockfree_queue_push(CTL_lockfree_queue *handle, const void *element);

CTL_API extern void CTL_lockfree_queue_pop(CTL_lockfree_queue *handle, void *element);

CTL_API extern size_t CTL_lockfree_queue_size(const CTL_lockfree_queue *handle);

CTL_API extern bool CTL_lockfree_queue_empty(const CTL_lockfree_queue *handle);