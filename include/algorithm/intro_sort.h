#pragma once

#include <stdbool.h>
#include <stddef.h>

extern void intro_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));