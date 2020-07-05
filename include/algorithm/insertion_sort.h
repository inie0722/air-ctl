#pragma once

#include <stdbool.h>
#include <stddef.h>

extern void insertion_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));

extern void unguarded_linear_insert(void *last, void *value, size_t T_size, bool (*compare)(const void *, const void *));