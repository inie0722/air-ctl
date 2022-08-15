/**
 * @file queue.h
 * @author 然Y (inie0722@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "air/ctl/container.h"
#include "air/ctl/config.h"

typedef struct
{
    CTL_container container;
    CTL_functions *functions;
} CTL_queue;

/**
 * @brief creates a new queue
 *
 * @param handle
 * @param functions container functions
 * @param data_size initial data size
 * @param T_size element type size
 */
CTL_API extern void CTL_queue_new(CTL_queue *handle, CTL_functions *functions, size_t data_size, size_t T_size);

/**
 * @brief destroys a queue
 *
 * @param handle
 */
CTL_API extern void CTL_queue_delete(CTL_queue *handle);

/**
 * @brief get first element
 *
 * @param handle
 * @return void* first element address
 */
CTL_API extern void *CTL_queue_front(const CTL_queue *handle);

/**
 * @brief get last element
 *
 * @param handle
 * @return void* last element address
 */
CTL_API extern void *CTL_queue_back(const CTL_queue *handle);

/**
 * @brief push element to the back
 *
 * @param handle
 * @param element element address
 */
CTL_API extern void CTL_queue_push(CTL_queue *handle, const void *element);

/**
 * @brief pop element from the front
 *
 * @param handle
 */
CTL_API extern void CTL_queue_pop(CTL_queue *handle);

/**
 * @brief the number of elements
 *
 * @param handle
 * @return size_t The number of elements in the container
 */
CTL_API extern size_t CTL_queue_size(const CTL_queue *handle);

/**
 * @brief if the queue is empty
 *
 * @param handle
 * @return true
 * @return false
 */
CTL_API extern bool CTL_queue_empty(const CTL_queue *handle);