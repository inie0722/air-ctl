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

#include "CTL/config.h"

/**
 * @brief sort elements
 * 
 * @param first 
 * @param last 
 * @param T_size element type size
 * @param compare compare function
 */
CTL_API extern void CTL_merge_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));

CTL_API extern void __CTL_merge(char *first_1, char *last_1, char *first_2, char *last_2, char *buf, size_t T_size, bool (*compare)(const void *, const void *));

CTL_API extern void __CTL_inplace_merge(char *first_1, char *last_1, char *first_2, char *last_2, size_t T_size, bool (*compare)(const void *, const void *));

CTL_API extern void __CTL_reverse(char *first, char *last, size_t T_size);