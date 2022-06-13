#include <stdlib.h>

#include "CTL/lockfree/spsc_queue.h"

static inline size_t get_index(size_t max_size, size_t index)
{
    return index % max_size;
}

void CTL_lockfree_spsc_queue_new(CTL_lockfree_spsc_queue *handle, size_t max_size, size_t T_size)
{
    handle->T_size = T_size;
    handle->max_size = max_size;
    handle->writable_limit = 0;
    handle->readable_limit = 0;
    handle->data = malloc(max_size * T_size);
}

void CTL_lockfree_spsc_queue_delete(CTL_lockfree_spsc_queue *handle)
{
    free(handle->data);
}