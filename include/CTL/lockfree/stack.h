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
} __CTL_lockfree_stack_node;

typedef struct
{
    size_t T_size;
    CTL_lockfree_allocator alloc;
    alignas(CTL_CACHE_LINE_SIZE) _Atomic CTL_aba_pointer top;
    alignas(CTL_CACHE_LINE_SIZE) atomic_size_t size;
} CTL_lockfree_stack;

#define CTL_ALLOCATOR_LOCK_FREE CTL_APA_POINTER_LOCK_FREE

/**
 * @brief create stack
 *
 * @param handle
 * @param T_size element type size
 */
CTL_API extern void CTL_lockfree_stack_new(CTL_lockfree_stack *handle, size_t T_size);

/**
 * @brief destroy stack
 *
 * @param handle
 */
CTL_API extern void CTL_lockfree_stack_delete(CTL_lockfree_stack *handle);

/**
 * @brief push element to the back
 *
 * @param handle
 * @param element element address
 * @return size_t number of push element
 */
CTL_API extern void CTL_lockfree_stack_push(CTL_lockfree_stack *handle, const void *element);

/**
 * @brief pop element from the back
 *
 * @param handle
 * @param element storage element address
 * @return size_t number of pop element
 */
CTL_API extern size_t CTL_lockfree_stack_pop(CTL_lockfree_stack *handle, void *element);

/**
 * @brief returns the number of elements
 *
 * @param handle
 * @return size_t The number of elements in the container
 */
CTL_API extern size_t CTL_lockfree_stack_size(const CTL_lockfree_stack *handle);

/**
 * @brief checks whether the underlying container is empty
 *
 * @param handle
 * @return true
 * @return false
 */
CTL_API extern bool CTL_lockfree_stack_empty(const CTL_lockfree_stack *handle);