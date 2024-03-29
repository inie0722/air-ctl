#include <string.h>
#include <stdlib.h>

#include "air/ctl/lockfree/mpmc_queue.h"
#include "air/ctl/compat/threads.h"
#include "air/ctl/malloc.h"

static inline size_t get_index(size_t max_size, size_t index)
{
    return index % max_size;
}

void CTL_lockfree_mpmc_queue_new(CTL_lockfree_mpmc_queue *handle, size_t max_size, size_t T_size)
{
    handle->T_size = T_size;
    handle->max_size = max_size;

    handle->data = CTL_malloc(max_size * T_size);
    handle->flag = CTL_malloc(max_size * sizeof(__CTL_lockfree_mpmc_queue_flag));

    for (size_t i = 0; i < max_size; i++)
    {
        atomic_init(&handle->flag[i].readable, 0);
        atomic_init(&handle->flag[i].writable, i);
    }

    atomic_init(&handle->readable_limit, 0);
    atomic_init(&handle->writable_limit, 0);
}

void CTL_lockfree_mpmc_queue_delete(CTL_lockfree_mpmc_queue *handle)
{
    CTL_free(handle->data, handle->max_size * handle->T_size);
    CTL_free(handle->flag, handle->max_size * sizeof(__CTL_lockfree_mpmc_queue_flag));
}

size_t CTL_lockfree_mpmc_queue_push(CTL_lockfree_mpmc_queue *handle, const void *element)
{
    size_t max_size = handle->max_size;
    size_t T_size = handle->T_size;
    char *data = handle->data;
    __CTL_lockfree_mpmc_queue_flag *flag = handle->flag;

    size_t index = atomic_fetch_add_explicit(&handle->writable_limit, 1, memory_order_relaxed);

    while (1)
    {
        size_t writable_flag = atomic_load_explicit(&flag[get_index(max_size, index)].writable, memory_order_acquire);

        if (writable_flag != index)
        {
            thrd_yield();
            continue;
        }
        else
            break;
    }

    memcpy(data + get_index(max_size, index) * T_size, element, T_size);
    atomic_store_explicit(&flag[get_index(max_size, index)].readable, index + 1, memory_order_release);
    return 1;
}

size_t CTL_lockfree_mpmc_queue_pop(CTL_lockfree_mpmc_queue *handle, void *element)
{
    size_t max_size = handle->max_size;
    size_t T_size = handle->T_size;
    char *data = handle->data;
    __CTL_lockfree_mpmc_queue_flag *flag = handle->flag;

    size_t index = atomic_fetch_add_explicit(&handle->readable_limit, 1, memory_order_relaxed);

    while (1)
    {
        size_t readable_flag = atomic_load_explicit(&flag[get_index(max_size, index)].readable, memory_order_acquire);

        if (readable_flag != index + 1)
        {
            thrd_yield();
            continue;
        }
        else
            break;
    }

    memcpy(element, data + get_index(max_size, index) * T_size, T_size);
    atomic_store_explicit(&flag[get_index(max_size, index)].writable, max_size + index, memory_order_release);

    return 1;
}

size_t CTL_lockfree_mpmc_queue_size(const CTL_lockfree_mpmc_queue *handle)
{
    size_t writable_limit = atomic_load_explicit(&handle->writable_limit, memory_order_relaxed);
    size_t readable_limit = atomic_load_explicit(&handle->readable_limit, memory_order_relaxed);

    return writable_limit > readable_limit ? writable_limit - readable_limit : 0;
}

bool CTL_lockfree_mpmc_queue_empty(const CTL_lockfree_mpmc_queue *handle)
{
    return !CTL_lockfree_mpmc_queue_size(handle);
}