#pragma once

#include "CTL/lockfree/aba_pointer.h"
#include "CTL/lockfree/allocator.h"

typedef struct
{
    _Atomic CTL_aba_pointer next;
    char data[];
} __CTL_lockfree_stack_node;

typedef struct
{
    CTL_lockfree_allocator alloc;
    _Atomic CTL_aba_pointer top;
    atomic_size_t size;
    size_t T_size;
} CTL_lockfree_stack;

CTL_API extern void CTL_lockfree_stack_new(CTL_lockfree_stack *handle, size_t T_size);

CTL_API extern void CTL_lockfree_stack_delete(CTL_lockfree_stack *handle);

CTL_API extern void CTL_lockfree_stack_push(CTL_lockfree_stack *handle, const void *element);

CTL_API extern void CTL_lockfree_stack_pop(CTL_lockfree_stack *handle, void *element);

CTL_API extern size_t CTL_lockfree_stack_size(const CTL_lockfree_stack *handle);

CTL_API extern bool CTL_lockfree_stack_empty(const CTL_lockfree_stack *handle);