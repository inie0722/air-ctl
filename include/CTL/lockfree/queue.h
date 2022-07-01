/**
 * @file queue.h
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

/**
 * @brief create queue
 *
 * @param handle
 * @param T_size element type size
 */
CTL_API extern void CTL_lockfree_queue_new(CTL_lockfree_queue *handle, size_t T_size);

/**
 * @brief destroy queue
 *
 * @param handle
 */
CTL_API extern void CTL_lockfree_queue_delete(CTL_lockfree_queue *handle);

/**
 * @brief push element to the back
 *
 * @param handle
 * @param element element address
 * @return size_t number of push element
 */
CTL_API extern size_t CTL_lockfree_queue_push(CTL_lockfree_queue *handle, const void *element);

/**
 * @brief pop element from the front
 *
 * @param handle
 * @param element storage element address
 * @return size_t number of pop element
 */
CTL_API extern size_t CTL_lockfree_queue_pop(CTL_lockfree_queue *handle, void *element);

/**
 * @brief returns the number of elements
 *
 * @param handle
 * @return size_t The number of elements in the container
 */
CTL_API extern size_t CTL_lockfree_queue_size(const CTL_lockfree_queue *handle);

/**
 * @brief checks whether the underlying container is empty
 *
 * @param handle
 * @return true
 * @return false
 */
CTL_API extern bool CTL_lockfree_queue_empty(const CTL_lockfree_queue *handle);