#include <string.h>

#include "air/ctl/lockfree/stack.h"
#include "air/ctl/compat/threads.h"

static inline __CTL_lockfree_stack_node *__aba_ptr_get(CTL_aba_pointer aba_ptr)
{
    return ((__CTL_lockfree_stack_node *)CTL_aba_pointer_get(aba_ptr));
}

void CTL_lockfree_stack_new(CTL_lockfree_stack *handle, size_t T_size)
{
    CTL_lockfree_allocator_new(&handle->alloc, sizeof(__CTL_lockfree_stack_node) + T_size);

    handle->T_size = T_size;
    atomic_init(&handle->size, 0);

    CTL_aba_pointer_atomic_init(&handle->top, CTL_aba_pointer_make(NULL));
}

void CTL_lockfree_stack_delete(CTL_lockfree_stack *handle)
{
    for (CTL_aba_pointer top = CTL_aba_pointer_atomic_load(&handle->top, memory_order_acquire);
         __aba_ptr_get(top);
         top = CTL_aba_pointer_atomic_load(&handle->top, memory_order_acquire))
    {
        CTL_aba_pointer next = __aba_ptr_get(top)->next;

        CTL_lockfree_deallocate(&handle->alloc, top);
        CTL_aba_pointer_atomic_store(&handle->top, next, memory_order_release);
    }

    CTL_lockfree_allocator_delete(&handle->alloc, sizeof(__CTL_lockfree_stack_node) + handle->T_size);
}

size_t CTL_lockfree_stack_push(CTL_lockfree_stack *handle, const void *element)
{
    CTL_aba_pointer node = CTL_lockfree_allocate(&handle->alloc);
    memcpy(__aba_ptr_get(node)->data, element, handle->T_size);

    CTL_aba_pointer exp = CTL_aba_pointer_atomic_load(&handle->top, memory_order_acquire);
    CTL_aba_pointer_atomic_store(&__aba_ptr_get(node)->next, exp, memory_order_relaxed);

    while (!CTL_aba_pointer_atomic_weak(&handle->top, &exp, node, memory_order_release, memory_order_relaxed))
    {
        CTL_aba_pointer_atomic_store(&__aba_ptr_get(node)->next, exp, memory_order_relaxed);
    }

    atomic_fetch_add_explicit(&handle->size, 1, memory_order_relaxed);

    return 1;
}

size_t CTL_lockfree_stack_pop(CTL_lockfree_stack *handle, void *element)
{
    CTL_aba_pointer exp = CTL_aba_pointer_atomic_load(&handle->top, memory_order_acquire);
    CTL_aba_pointer ret = exp;

    while (__aba_ptr_get(exp) == NULL ||
           !CTL_aba_pointer_atomic_weak(&handle->top, &exp, __aba_ptr_get(exp)->next, memory_order_release, memory_order_relaxed))
    {
        while (__aba_ptr_get(exp) == NULL)
        {
            thrd_yield();
            exp = CTL_aba_pointer_atomic_load(&handle->top, memory_order_acquire);
        }

        ret = exp;
    }

    atomic_fetch_sub_explicit(&handle->size, 1, memory_order_relaxed);
    memcpy(element, __aba_ptr_get(ret)->data, handle->T_size);
    CTL_lockfree_deallocate(&handle->alloc, ret);

    return 1;
}

size_t CTL_lockfree_stack_size(const CTL_lockfree_stack *handle)
{
    return atomic_load_explicit(&handle->size, memory_order_relaxed);
}

bool CTL_lockfree_stack_empty(const CTL_lockfree_stack *handle)
{
    return !CTL_lockfree_stack_size(handle);
}