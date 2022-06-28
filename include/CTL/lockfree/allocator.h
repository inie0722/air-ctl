#pragma once

#include <stddef.h>

#include "CTL/config.h"
#include "CTL/lockfree/aba_pointer.h"

typedef union
{
    _Atomic CTL_aba_pointer next;
    char data[0];
} __CTL_lockfree_allocator_node;

typedef struct
{
    _Atomic CTL_aba_pointer free_list;
    size_t T_size;
} CTL_lockfree_allocator;

CTL_API extern void CTL_lockfree_allocator_new(CTL_lockfree_allocator *handle, size_t T_size);

CTL_API extern void CTL_lockfree_allocator_delete(CTL_lockfree_allocator *handle, size_t T_size);

CTL_API extern CTL_aba_pointer CTL_lockfree_allocator_allocate(CTL_lockfree_allocator *handle);

CTL_API extern void CTL_lockfree_allocator_deallocate(CTL_lockfree_allocator *handle, CTL_aba_pointer ptr);