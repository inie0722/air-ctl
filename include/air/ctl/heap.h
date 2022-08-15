/**
 * @file heap.h
 * @author 然Y (inie0722@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include <stdbool.h>

#include "air/ctl/container.h"
#include "air/ctl/config.h"

/**
 * @brief push element
 * 
 * @param functions container functions
 * @param first first iterator
 * @param last last iterator
 * @param compare_handler compare function
 */
CTL_API extern void CTL_push_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(const void *left, const void *right));

/**
 * @brief pop element
 * 
 * @param functions container functions
 * @param first first iterator
 * @param last last iterator
 * @param compare_handler compare function
 */
CTL_API extern void CTL_pop_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(const void *left, const void *right));

/**
 * @brief sort elements
 * 
 * @param functions container functions
 * @param first first iterator
 * @param last last iterator
 * @param compare_handler compare function
 */
CTL_API extern void CTL_sort_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(const void *left, const void *right));

/**
 * @brief make heap
 * 
 * @param functions container functions
 * @param first first iterator
 * @param last last iterator
 * @param compare_handler compare function
 */
CTL_API extern void CTL_make_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(const void *left, const void *right));