/**
 * @file stack.h
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
#include "CTL/config.h"

/**
 * @brief stack
 *
 */
typedef struct
{
    CTL_container container;
    CTL_functions *functions;

} CTL_stack;

/**
 * @brief create stack
 *
 * @param handle
 * @param functions container functions
 * @param data_size initial data size
 * @param T_size element type size
 */
CTL_API extern void CTL_stack_new(CTL_stack *handle, CTL_functions *functions, size_t buf_size, size_t T_size);

/**
 * @brief destroy stack
 *
 * @param handle
 */
CTL_API extern void CTL_stack_delete(CTL_stack *handle);

/**
 * @brief get top element
 *
 * @param handle
 * @return void* top element address
 */
CTL_API extern void *CTL_stack_top(const CTL_stack *handle);

/**
 * @brief push element
 *
 * @param handle
 * @param element element address
 */
CTL_API extern void CTL_stack_push(CTL_stack *handle, const void *element);

/**
 * @brief pop element
 *
 * @param handle
 */
CTL_API extern void CTL_stack_pop(CTL_stack *handle);

/**
 * @brief get the number of elements that can be held in currently allocated storage
 *
 * @param handle
 * @return size_t number of elements that can be held in currently allocated storage
 */
CTL_API extern size_t CTL_stack_size(const CTL_stack *handle);

/**
 * @brief if the stack is empty
 *
 * @param handle
 * @return true
 * @return false
 */
CTL_API extern bool CTL_stack_empty(const CTL_stack *handle);