/**
 * @file heap_sort.h
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