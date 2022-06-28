#include <stdlib.h>

#include "CTL/lockfree/allocator.h"

void CTL_lockfree_allocator_new(CTL_lockfree_allocator *handle, size_t T_size)
{
    handle->free_list = CTL_aba_pointer_make(NULL);
    handle->T_size = T_size;
}

void CTL_lockfree_allocator_delete(CTL_lockfree_allocator *handle, size_t T_size)
{
    while (1)
    {
        CTL_aba_pointer free_list = CTL_aba_pointer_atomic_load(&handle->free_list, memory_order_seq_cst);

        if (CTL_aba_pointer_get(free_list) == NULL)
        {
            break;
        }

        CTL_aba_pointer next = ((__CTL_lockfree_allocator_node *)CTL_aba_pointer_get(free_list))->next;
        free(CTL_aba_pointer_get(free_list));

        CTL_aba_pointer_atomic_store(&handle->free_list, next, memory_order_seq_cst);
    }
}

CTL_aba_pointer CTL_lockfree_allocator_allocate(CTL_lockfree_allocator *handle)
{
    CTL_aba_pointer exp = CTL_aba_pointer_atomic_load(&handle->free_list, memory_order_seq_cst);

    while (1)
    {
        if (!CTL_aba_pointer_get(exp))
        {
            return CTL_aba_pointer_make(malloc(handle->T_size));
        }

        CTL_aba_pointer next = ((__CTL_lockfree_allocator_node *)CTL_aba_pointer_get(exp))->next;

        if (CTL_aba_pointer_atomic_weak(&handle->free_list, &exp, next, memory_order_seq_cst))
        {
            return exp;
        }
    }
}

void CTL_lockfree_allocator_deallocate(CTL_lockfree_allocator *handle, CTL_aba_pointer ptr)
{

    while (1)
    {
        CTL_aba_pointer exp = CTL_aba_pointer_atomic_load(&handle->free_list, memory_order_seq_cst);
        ((__CTL_lockfree_allocator_node *)CTL_aba_pointer_get(ptr))->next = exp;

        if (CTL_aba_pointer_atomic_weak(&handle->free_list, &exp, ptr, memory_order_seq_cst))
            return;
    }
}