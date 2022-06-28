#pragma once

#include "CTL/lockfree/aba_pointer.h"
#include "CTL/lockfree/allocator.h"

typedef struct
{
    _Atomic CTL_aba_pointer next;
    char data[];
} __CTL_lockfree_queue_node;

typedef struct
{
    CTL_lockfree_allocator alloc;
    _Atomic CTL_aba_pointer head;
    _Atomic CTL_aba_pointer tail;
    atomic_size_t size;
    size_t T_size;
} CTL_lockfree_queue;

CTL_API extern void CTL_lockfree_queue_new(CTL_lockfree_queue *handle, size_t T_size);

CTL_API extern void CTL_lockfree_queue_delete(CTL_lockfree_queue *handle);

CTL_API extern void CTL_lockfree_queue_push(CTL_lockfree_queue *handle, const void *element);

CTL_API extern void CTL_lockfree_queue_pop(CTL_lockfree_queue *handle, void *element);

CTL_API extern size_t CTL_lockfree_queue_size(const CTL_lockfree_queue *handle);

CTL_API extern bool CTL_lockfree_queue_empty(const CTL_lockfree_queue *handle);