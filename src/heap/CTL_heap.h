#pragma once

#include "../vector/CTL_vector.h"

typedef int type;

void CTL_push_heap(CTL_vector_iterator *first, CTL_vector_iterator *last);

void CTL_pop_heap(CTL_vector_iterator *first, CTL_vector_iterator *last);

void CTL_sort_heap(CTL_vector_iterator *first, CTL_vector_iterator *last);

void CTL_make_heap(CTL_vector_iterator *first, CTL_vector_iterator *last);