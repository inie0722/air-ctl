#pragma once

#include <stddef.h>
#include <stdalign.h>
#include <stdbool.h>

#include "CTL/config.h"
#include "CTL/compat/atomic.h"

typedef struct
{
    char *data;
    size_t T_size;
    size_t max_size;
    alignas(CTL_CACHE_LINE_SIZE) atomic_size_t writable_limit;
    alignas(CTL_CACHE_LINE_SIZE) atomic_size_t readable_limit;
} CTL_lockfree_spsc_queue;

#define CTL_SPSC_QUEUE_LOCK_FREE ATOMIC_LONG_LOCK_FREE

/**
 * @brief create spsc queue
 *
 * @param handle
 * @param max_size
 * @param T_size element type size
 */
CTL_API extern void CTL_lockfree_spsc_queue_new(CTL_lockfree_spsc_queue *handle, size_t max_size, size_t T_size);

/**
 * @brief destroy spsc queue
 *
 * @param handle
 */
CTL_API extern void CTL_lockfree_spsc_queue_delete(CTL_lockfree_spsc_queue *handle);

/**
 * @brief push element to the back
 *
 * @param handle
 * @param element element address
 * @return size_t number of push element
 */
CTL_API extern size_t CTL_lockfree_spsc_queue_push(CTL_lockfree_spsc_queue *handle, const void *first, size_t count);

/**
 * @brief pop element from the front
 *
 * @param handle
 * @param element storage element address
 * @return size_t number of pop element
 */
CTL_API extern size_t CTL_lockfree_spsc_queue_pop(CTL_lockfree_spsc_queue *handle, void *first, size_t count);

/**
 * @brief returns the number of elements
 *
 * @param handle
 * @return size_t The number of elements in the container
 */
CTL_API extern size_t CTL_lockfree_spsc_queue_size(const CTL_lockfree_spsc_queue *handle);

/**
 * @brief checks whether the underlying container is empty
 *
 * @param handle
 * @return true
 * @return false
 */
CTL_API extern bool CTL_lockfree_spsc_queue_empty(const CTL_lockfree_spsc_queue *handle);