/**
 * @file merge_sort.h
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
extern void merge_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));

extern void merge(void *first_1, void *last_1, void *first_2, void *last_2, void *buf, size_t T_size, bool (*compare)(const void *, const void *));

extern void inplace_merge(void *first_1, void *last_1, void *first_2, void *last_2, size_t T_size, bool (*compare)(const void *, const void *));

extern void reverse(void *first, void *last, size_t T_size);