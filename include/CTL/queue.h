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

#include "CTL/container.h"

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
extern void CTL_queue_new(CTL_queue *handle, CTL_functions *functions, const size_t data_size, const size_t T_size);

/**
 * @brief destroys a queue
 *
 * @param handle
 */
extern void CTL_queue_delete(CTL_queue *handle);

/**
 * @brief get first element
 *
 * @param handle
 * @return void* first element address
 */
extern void *CTL_queue_front(const CTL_queue *handle);

/**
 * @brief get last element
 *
 * @param handle
 * @return void* last element address
 */
extern void *CTL_queue_back(const CTL_queue *handle);

/**
 * @brief push element to the back
 *
 * @param handle
 * @param element element address
 */
extern void CTL_queue_push(CTL_queue *handle, const void *element);

/**
 * @brief pop element from the front
 *
 * @param handle
 */
extern void CTL_queue_pop(CTL_queue *handle);

/**
 * @brief get the number of elements that can be held in currently allocated storage
 *
 * @param handle
 * @return size_t number of elements that can be held in currently allocated storage
 */
extern size_t CTL_queue_size(const CTL_queue *handle);

/**
 * @brief if the queue is empty
 *
 * @param handle
 * @return true
 * @return false
 */
extern bool CTL_queue_empty(const CTL_queue *handle);