#pragma once

#include <stdbool.h>
#include <stddef.h>

extern void quick_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));

extern void *median(void *a, void *b, void *c, bool (*compare)(const void *, const void *));

extern void *unguarded_partition(void *first, void *last, void *value, size_t T_size, bool (*compare)(const void *, const void *));