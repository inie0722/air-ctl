/**
 * @file quick_sort.h
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
 * @brief sort elements
 * 
 * @param first 
 * @param last 
 * @param T_size element type size
 * @param compare compare function
 */
extern void quick_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));

extern void *median(void *a, void *b, void *c, bool (*compare)(const void *, const void *));

extern void *unguarded_partition(void *first, void *last, void *value, size_t T_size, bool (*compare)(const void *, const void *));