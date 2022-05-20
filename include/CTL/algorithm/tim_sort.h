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
extern void tim_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));