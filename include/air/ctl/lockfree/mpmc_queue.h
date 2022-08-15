/**
 * @file mpmc_queue.h
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
#include <stdbool.h>

#include "air/ctl/config.h"
#include "air/ctl/compat/atomic.h"

typedef struct
{
    CTL_ALIGNAS(CTL_CACHE_LINE_SIZE) atomic_size_t writable;
    CTL_ALIGNAS(CTL_CACHE_LINE_SIZE) atomic_size_t readable;
} __CTL_lockfree_mpmc_queue_flag;

typedef struct
{
    char *data;
    size_t T_size;
    size_t max_size;
    __CTL_lockfree_mpmc_queue_flag *flag;
    CTL_ALIGNAS(CTL_CACHE_LINE_SIZE) atomic_size_t writable_limit;
    CTL_ALIGNAS(CTL_CACHE_LINE_SIZE) atomic_size_t readable_limit;
} CTL_lockfree_mpmc_queue;

#define CTL_MPMC_QUEUE_LOCK_FREE ATOMIC_LONG_LOCK_FREE

/**
 * @brief create mpmc queue
 *
 * @param handle
 * @param max_size
 * @param T_size element type size
 */
CTL_API extern void CTL_lockfree_mpmc_queue_new(CTL_lockfree_mpmc_queue *handle, size_t max_size, size_t T_size);

/**
 * @brief destroy mpmc queue
 *
 * @param handle
 */
CTL_API extern void CTL_lockfree_mpmc_queue_delete(CTL_lockfree_mpmc_queue *handle);

/**
 * @brief push element to the back
 *
 * @param handle
 * @param element element address
 * @return size_t number of push element
 */
CTL_API extern size_t CTL_lockfree_mpmc_queue_push(CTL_lockfree_mpmc_queue *handle, const void *element);

/**
 * @brief pop element from the front
 *
 * @param handle
 * @param element storage element address
 * @return size_t number of pop element
 */
CTL_API extern size_t CTL_lockfree_mpmc_queue_pop(CTL_lockfree_mpmc_queue *handle, void *element);

/**
 * @brief returns the number of elements
 *
 * @param handle
 * @return size_t The number of elements in the container
 */
CTL_API extern size_t CTL_lockfree_mpmc_queue_size(const CTL_lockfree_mpmc_queue *handle);

/**
 * @brief checks whether the underlying container is empty
 *
 * @param handle
 * @return true
 * @return false
 */
CTL_API extern bool CTL_lockfree_mpmc_queue_empty(const CTL_lockfree_mpmc_queue *handle);