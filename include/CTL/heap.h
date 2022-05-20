#pragma once

#include <stdbool.h>
#include "CTL/container.h"

/**
 * @brief push element
 * 
 * @param functions container functions
 * @param first first iterator
 * @param last last iterator
 * @param compare_handler compare function
 */
extern void CTL_push_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(void *a, void *b));

/**
 * @brief pop element
 * 
 * @param functions container functions
 * @param first first iterator
 * @param last last iterator
 * @param compare_handler compare function
 */
extern void CTL_pop_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(void *a, void *b));

/**
 * @brief sort elements
 * 
 * @param functions container functions
 * @param first first iterator
 * @param last last iterator
 * @param compare_handler compare function
 */
extern void CTL_sort_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(void *a, void *b));

/**
 * @brief make heap
 * 
 * @param functions container functions
 * @param first first iterator
 * @param last last iterator
 * @param compare_handler compare function
 */
extern void CTL_make_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(void *a, void *b));