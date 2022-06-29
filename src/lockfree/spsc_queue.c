#include <stdlib.h>
#include <string.h>

#include "CTL/lockfree/spsc_queue.h"
#include "CTL/allocator.h"

static inline size_t get_index(size_t max_size, size_t index)
{
    return index % max_size;
}

void CTL_lockfree_spsc_queue_new(CTL_lockfree_spsc_queue *handle, size_t max_size, size_t T_size)
{
    handle->T_size = T_size;
    handle->max_size = max_size;
    handle->data = CTL_allocate(max_size * T_size);

    atomic_init(&handle->writable_limit, 0);
    atomic_init(&handle->readable_limit, 0);
}

void CTL_lockfree_spsc_queue_delete(CTL_lockfree_spsc_queue *handle)
{
    CTL_deallocate(handle->data, handle->max_size * handle->T_size);
}

size_t CTL_lockfree_spsc_queue_push(CTL_lockfree_spsc_queue *handle, const void *first, size_t count)
{
    size_t max_size = handle->max_size;
    size_t T_size = handle->T_size;
    char *data = handle->data;
    size_t writable_limit = atomic_load_explicit(&handle->writable_limit, memory_order_relaxed);

    //可写数量
    size_t available = max_size - (writable_limit - atomic_load_explicit(&handle->readable_limit, memory_order_acquire));
    count = available < count ? available : count;

    if (count == 0)
    {
        return 0;
    }

    if ((writable_limit % max_size) + count > max_size)
    {
        size_t len = max_size - (writable_limit % max_size);

        memcpy(data + get_index(max_size, writable_limit) * T_size, first, len * T_size);
        memcpy(data, (char *)first + len * T_size, count - len * T_size);
    }
    else
    {
        memcpy(data + get_index(max_size, writable_limit) * T_size, first, count * T_size);
    }

    atomic_fetch_add_explicit(&handle->writable_limit, count, memory_order_release);

    return count;
}

size_t CTL_lockfree_spsc_queue_pop(CTL_lockfree_spsc_queue *handle, void *result, size_t count)
{
    size_t max_size = handle->max_size;
    size_t T_size = handle->T_size;
    char *data = handle->data;
    size_t readable_limit = atomic_load_explicit(&handle->readable_limit, memory_order_relaxed);

    //可读数量
    size_t available = atomic_load_explicit(&handle->writable_limit, memory_order_acquire) - readable_limit;
    count = available < count ? available : count;

    if (count == 0)
    {
        return 0;
    }

    if ((readable_limit % max_size) + count > max_size)
    {
        size_t len = max_size - (readable_limit % max_size);

        memcpy(result, data + get_index(max_size, readable_limit) * T_size, len * T_size);
        memcpy((char *)result + len * T_size, data, count - len * T_size);
    }
    else
    {
        memcpy(result, data + get_index(max_size, readable_limit) * T_size, count * T_size);
    }

    atomic_fetch_add_explicit(&handle->readable_limit, count, memory_order_release);

    return count;
}

size_t CTL_lockfree_spsc_queue_size(const CTL_lockfree_spsc_queue *handle)
{
    return atomic_load_explicit(&handle->writable_limit, memory_order_acquire) - atomic_load_explicit(&handle->readable_limit, memory_order_acquire);
}

bool CTL_lockfree_spsc_queue_empty(const CTL_lockfree_spsc_queue *handle)
{
    return !CTL_lockfree_spsc_queue_size(handle);
}