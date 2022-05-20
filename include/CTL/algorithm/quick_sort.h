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