/**
 * @file priority_queue.h
 * @author 然Y (inie0722@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "CTL/container.h"
#include "CTL/config.h"

/**
 * @brief priority queue
 *
 */
typedef struct
{
    CTL_container container;
    CTL_functions *functions;
    bool (*compare_handler)(const void *left, const void *right);
} CTL_priority_queue;

/**
 * @brief creates a new priority queue
 *
 * @param handle
 * @param compare_handler compare function
 * @param functions container functions
 * @param data_size initial data size
 * @param T_size element type size
 */
CTL_API extern void CTL_priority_queue_new(CTL_priority_queue *handle, bool (*compare_handler)(const void *left, const void *right), CTL_functions *functions, size_t data_size, size_t T_size);

/**
 * @brief destroys a priority queue
 *
 * @param handle
 */
CTL_API extern void CTL_priority_queue_delete(CTL_priority_queue *handle);

/**
 * @brief get top element
 *
 * @param handle
 * @return void* top element address
 */
CTL_API extern void *CTL_priority_queue_top(const CTL_priority_queue *handle);

/**
 * @brief push element
 *
 * @param handle
 * @param element element address
 */
CTL_API extern void CTL_priority_queue_push(CTL_priority_queue *handle, const void *element);

/**
 * @brief pop element
 *
 * @param handle
 */
CTL_API extern void CTL_priority_queue_pop(CTL_priority_queue *handle);

/**
 * @brief the number of elements
 *
 * @param handle
 * @return size_t The number of elements in the container
 */
CTL_API extern size_t CTL_priority_queue_size(const CTL_priority_queue *handle);

/**
 * @brief if priority queue is empty
 *
 * @param handle
 * @return true
 * @return false
 */
CTL_API extern bool CTL_priority_queue_empty(const CTL_priority_queue *handle);