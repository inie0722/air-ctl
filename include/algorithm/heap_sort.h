#pragma once

#include <stdbool.h>
#include <stddef.h>

extern void heap_push(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));

extern void heap_pop(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));

extern void heap_make(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));

extern void heap_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));