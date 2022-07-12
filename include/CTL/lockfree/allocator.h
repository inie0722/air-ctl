/**
 * @file allocator.h
 * @author 然Y (inie0722@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <stddef.h>

#include "CTL/config.h"
#include "CTL/lockfree/aba_pointer.h"

typedef union
{
    CTL_ALIGNAS(CTL_CACHE_LINE_SIZE) _Atomic CTL_aba_pointer next;
    char data[0];
} __CTL_lockfree_allocator_node;

typedef struct
{
    size_t T_size;
    CTL_ALIGNAS(CTL_CACHE_LINE_SIZE) _Atomic CTL_aba_pointer free_list;
} CTL_lockfree_allocator;

#define CTL_ALLOCATOR_LOCK_FREE CTL_APA_POINTER_LOCK_FREE

CTL_API extern void CTL_lockfree_allocator_new(CTL_lockfree_allocator *handle, size_t T_size);

CTL_API extern void CTL_lockfree_allocator_delete(CTL_lockfree_allocator *handle, size_t T_size);

CTL_API extern CTL_aba_pointer CTL_lockfree_allocate(CTL_lockfree_allocator *handle);

CTL_API extern void CTL_lockfree_deallocate(CTL_lockfree_allocator *handle, CTL_aba_pointer ptr);