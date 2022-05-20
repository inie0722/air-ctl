#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief push element
 * 
 * @param first 
 * @param last 
 * @param T_size element type size
 * @param compare compare function
 */
extern void CTL_heap_push(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));

/**
 * @brief pop element
 * 
 * @param first 
 * @param last 
 * @param T_size element type size
 * @param compare compare function
 */
extern void CTL_heap_pop(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));

/**
 * @brief make heap
 * 
 * @param first 
 * @param last 
 * @param T_size element type size
 * @param compare compare function
 */
extern void CTL_heap_make(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));

/**
 * @brief sort elements
 * 
 * @param first 
 * @param last 
 * @param T_size element type size
 * @param compare compare function
 */
extern void CTL_heap_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));