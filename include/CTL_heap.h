#pragma once

#include <stdbool.h>
#include "CTL_container.h"

void CTL_push_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(void *a, void *b));

void CTL_pop_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(void *a, void *b));

void CTL_sort_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(void *a, void *b));

void CTL_make_heap(CTL_functions *functions, CTL_iterator *first, CTL_iterator *last, bool (*compare_handler)(void *a, void *b));