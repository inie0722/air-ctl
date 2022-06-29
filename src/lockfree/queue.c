#include <string.h>

#include "CTL/lockfree/queue.h"

static inline __CTL_lockfree_queue_node *__aba_ptr_get(CTL_aba_pointer aba_ptr)
{
    return ((__CTL_lockfree_queue_node *)CTL_aba_pointer_get(aba_ptr));
}

void CTL_lockfree_queue_new(CTL_lockfree_queue *handle, size_t T_size)
{
    CTL_lockfree_allocator_new(&handle->alloc, sizeof(__CTL_lockfree_queue_node) + T_size);

    handle->T_size = T_size;
    handle->size = 0;

    //先构造一个头节点
    CTL_aba_pointer node = CTL_lockfree_allocate(&handle->alloc);
    CTL_aba_pointer_atomic_init(&__aba_ptr_get(node)->next, CTL_aba_pointer_make(NULL));

    CTL_aba_pointer_atomic_init(&handle->head, node);
    CTL_aba_pointer_atomic_init(&handle->tail, node);
}

void CTL_lockfree_queue_delete(CTL_lockfree_queue *handle)
{
    for (CTL_aba_pointer head = CTL_aba_pointer_atomic_load(&handle->head, memory_order_acquire);
         __aba_ptr_get(head);
         head = CTL_aba_pointer_atomic_load(&handle->head, memory_order_acquire))
    {
        CTL_aba_pointer next = CTL_aba_pointer_atomic_load(&__aba_ptr_get(head)->next, memory_order_acquire);

        CTL_lockfree_deallocate(&handle->alloc, head);
        CTL_aba_pointer_atomic_store(&handle->head, next, memory_order_release);
    }

    CTL_lockfree_allocator_delete(&handle->alloc, sizeof(__CTL_lockfree_queue_node) + handle->T_size);
}

void CTL_lockfree_queue_push(CTL_lockfree_queue *handle, const void *element)
{
    CTL_aba_pointer node = CTL_lockfree_allocate(&handle->alloc);
    __CTL_lockfree_queue_node *node_ptr = __aba_ptr_get(node);
    node_ptr->next = CTL_aba_pointer_make(NULL);
    memcpy(node_ptr->data, element, handle->T_size);

    while (1)
    {
        CTL_aba_pointer tail = CTL_aba_pointer_atomic_load(&handle->tail, memory_order_acquire);
        CTL_aba_pointer next = CTL_aba_pointer_atomic_load(&__aba_ptr_get(tail)->next, memory_order_acquire);
        CTL_aba_pointer tail2 = CTL_aba_pointer_atomic_load(&handle->tail, memory_order_acquire);

        if (__aba_ptr_get(tail) == __aba_ptr_get(tail2))
        {
            if (__aba_ptr_get(next) == NULL)
            {
                if (CTL_aba_pointer_atomic_weak(&__aba_ptr_get(tail)->next, &next, node, memory_order_release, memory_order_relaxed))
                {
                    CTL_aba_pointer_atomic_strong(&handle->tail, &tail, node, memory_order_release, memory_order_relaxed);
                    atomic_fetch_add_explicit(&handle->size, 1, memory_order_relaxed);
                    return;
                }
            }
            else
            {
                CTL_aba_pointer_atomic_weak(&handle->tail, &tail, next, memory_order_release, memory_order_relaxed);
            }
        }
    }
}

void CTL_lockfree_queue_pop(CTL_lockfree_queue *handle, void *element)
{
    while (1)
    {
        CTL_aba_pointer head = CTL_aba_pointer_atomic_load(&handle->head, memory_order_acquire);
        CTL_aba_pointer tail = CTL_aba_pointer_atomic_load(&handle->tail, memory_order_acquire);
        CTL_aba_pointer next = CTL_aba_pointer_atomic_load(&__aba_ptr_get(head)->next, memory_order_acquire);
        CTL_aba_pointer head2 = CTL_aba_pointer_atomic_load(&handle->head, memory_order_acquire);

        if (__aba_ptr_get(head) == __aba_ptr_get(head2))
        {
            if (__aba_ptr_get(head) == __aba_ptr_get(tail))
            {
                if (__aba_ptr_get(next) == NULL)
                    continue;
                CTL_aba_pointer_atomic_strong(&handle->tail, &tail, next, memory_order_release, memory_order_relaxed);
            }
            else
            {
                if (__aba_ptr_get(next) == NULL)
                    continue;

                memcpy(element, __aba_ptr_get(next)->data, handle->T_size);
                if (CTL_aba_pointer_atomic_weak(&handle->head, &head, next, memory_order_release, memory_order_relaxed))
                {
                    atomic_fetch_sub_explicit(&handle->size, 1, memory_order_relaxed);
                    CTL_lockfree_deallocate(&handle->alloc, head);
                    return;
                }
            }
        }
    }
}

size_t CTL_lockfree_queue_size(const CTL_lockfree_queue *handle)
{
    return atomic_load_explicit(&handle->size, memory_order_relaxed);
}

bool CTL_lockfree_queue_empty(const CTL_lockfree_queue *handle)
{
    return !atomic_load_explicit(&handle->size, memory_order_relaxed);
}