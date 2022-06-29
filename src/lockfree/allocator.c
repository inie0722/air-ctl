#include "CTL/lockfree/allocator.h"
#include "CTL/allocator.h"

static inline __CTL_lockfree_allocator_node *__aba_ptr_get(CTL_aba_pointer aba_ptr)
{
    return ((__CTL_lockfree_allocator_node *)CTL_aba_pointer_get(aba_ptr));
}

void CTL_lockfree_allocator_new(CTL_lockfree_allocator *handle, size_t T_size)
{
    handle->T_size = T_size;

    CTL_aba_pointer_atomic_init(&handle->free_list, CTL_aba_pointer_make(NULL));
}

void CTL_lockfree_allocator_delete(CTL_lockfree_allocator *handle, size_t T_size)
{
    while (1)
    {
        CTL_aba_pointer free_list = CTL_aba_pointer_atomic_load(&handle->free_list, memory_order_acquire);

        if (CTL_aba_pointer_get(free_list) == NULL)
        {
            break;
        }

        CTL_aba_pointer next = __aba_ptr_get(free_list)->next;
        CTL_deallocate(__aba_ptr_get(free_list), handle->T_size);

        CTL_aba_pointer_atomic_store(&handle->free_list, next, memory_order_release);
    }
}

CTL_aba_pointer CTL_lockfree_allocate(CTL_lockfree_allocator *handle)
{
    CTL_aba_pointer exp = CTL_aba_pointer_atomic_load(&handle->free_list, memory_order_acquire);

    while (1)
    {
        if (!CTL_aba_pointer_get(exp))
        {
            return CTL_aba_pointer_make(CTL_allocate(handle->T_size));
        }

        CTL_aba_pointer next = __aba_ptr_get(exp)->next;

        if (CTL_aba_pointer_atomic_weak(&handle->free_list, &exp, next, memory_order_release, memory_order_relaxed))
        {
            return exp;
        }
    }
}

void CTL_lockfree_deallocate(CTL_lockfree_allocator *handle, CTL_aba_pointer ptr)
{
    while (1)
    {
        CTL_aba_pointer exp = CTL_aba_pointer_atomic_load(&handle->free_list, memory_order_acquire);
        __aba_ptr_get(ptr)->next = exp;

        if (CTL_aba_pointer_atomic_weak(&handle->free_list, &exp, ptr, memory_order_release, memory_order_relaxed))
            return;
    }
}