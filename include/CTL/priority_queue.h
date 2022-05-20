#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "CTL/container.h"

/**
 * @brief priority queue
 *
 */
typedef struct
{
    CTL_container container;
    CTL_functions *functions;
    bool (*compare_handler)(void *a, void *b);
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
void CTL_priority_queue_new(CTL_priority_queue *handle, bool (*compare_handler)(void *a, void *b), CTL_functions *functions, const size_t data_size, const size_t T_size);

/**
 * @brief destroys a priority queue
 *
 * @param handle
 */
void CTL_priority_queue_delete(CTL_priority_queue *handle);

/**
 * @brief get top element
 *
 * @param handle
 * @return void* top element address
 */
void *CTL_priority_queue_top(const CTL_priority_queue *handle);

/**
 * @brief push element
 *
 * @param handle
 * @param element element address
 */
void CTL_priority_queue_push(CTL_priority_queue *handle, const void *element);

/**
 * @brief pop element
 *
 * @param handle
 */
void CTL_priority_queue_pop(CTL_priority_queue *handle);

/**
 * @brief get the number of elements that can be held in currently allocated storage
 *
 * @param handle
 * @return size_t number of elements that can be held in currently allocated storage
 */
size_t CTL_priority_queue_size(const CTL_priority_queue *handle);

/**
 * @brief if priority queue is empty
 *
 * @param handle
 * @return true
 * @return false
 */
bool CTL_priority_queue_empty(const CTL_priority_queue *handle);