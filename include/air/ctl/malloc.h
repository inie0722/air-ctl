/**
 * @file allocator.h
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

#include "air/ctl/config.h"

/**
 * @brief set memory allocation failure handling operation
 *
 * @param handler memory allocation failure handling operation
 */
CTL_API extern void CTL_set_malloc_handler(void (*handler)());

/**
 * @brief allocate memory
 *
 * @param size memory size
 * @return void* memory address
 */
CTL_API extern void *CTL_malloc(size_t size);

/**
 * @brief reallocate memory
 *
 * @param old_ptr old memory address
 * @param old_size old memory size
 * @param new_size new memory size
 * @return void* new memory address
 */
CTL_API extern void *CTL_remalloc(void *old_ptr, size_t old_size, size_t new_size);

/**
 * @brief free memory
 *
 * @param ptr memory address
 * @param size memory size
 */
CTL_API extern void CTL_free(void *ptr, size_t size);