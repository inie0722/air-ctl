#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "CTL/compat/atomic.h"

#if defined __cplusplus
static_assert(sizeof(void *) == 8, "only support 64-bit");
#else
_Static_assert(sizeof(void *) == 8, "only support 64-bit");
#endif

#define __CTL_ABA_COUNT_MASK (0XFFFFull << 48)
#define __CTL_ABA_POINTER_MASK (~__CTL_ABA_COUNT_MASK)

#define CTL_APA_POINTER_LOCK_FREE ATOMIC_POINTER_LOCK_FREE

typedef uint64_t CTL_aba_pointer;

static inline CTL_aba_pointer CTL_aba_pointer_make(void *ptr)
{
    uint64_t new_ptr = (0ull << 48) + ((uintptr_t)ptr & __CTL_ABA_POINTER_MASK);

    return new_ptr;
}

static inline void *CTL_aba_pointer_get(CTL_aba_pointer ptr)
{
    return (void *)(ptr & __CTL_ABA_POINTER_MASK);
}

#if defined __STDC_VERSION__
static inline CTL_aba_pointer CTL_aba_pointer_atomic_load(const _Atomic CTL_aba_pointer *obj, memory_order order)
{
    return atomic_load_explicit(obj, order);
}

static inline void CTL_aba_pointer_atomic_store(_Atomic CTL_aba_pointer *obj, CTL_aba_pointer desired, memory_order order)
{
    uint16_t count = ((desired & __CTL_ABA_COUNT_MASK) >> 48) + 1;
    uint64_t new_ptr = ((uint64_t)count << 48) + (desired & __CTL_ABA_POINTER_MASK);

    atomic_store_explicit(obj, new_ptr, order);
}

static inline CTL_aba_pointer CTL_aba_pointer_atomic_exchange(_Atomic CTL_aba_pointer *obj, CTL_aba_pointer desired, memory_order order)
{
    uint16_t count = ((desired & __CTL_ABA_COUNT_MASK) >> 48) + 1;
    uint64_t new_ptr = ((uint64_t)count << 48) + (desired & __CTL_ABA_POINTER_MASK);

    return atomic_exchange_explicit(obj, new_ptr, order);
}

static inline bool CTL_aba_pointer_atomic_weak(_Atomic CTL_aba_pointer *obj, CTL_aba_pointer *expected, CTL_aba_pointer desired, memory_order order)
{
    uint16_t count = ((desired & __CTL_ABA_COUNT_MASK) >> 48) + 1;
    uint64_t new_ptr = ((uint64_t)count << 48) + (desired & __CTL_ABA_POINTER_MASK);

    return atomic_compare_exchange_weak_explicit(obj, expected, new_ptr, order, order);
}

static inline bool CTL_aba_pointer_atomic_strong(_Atomic CTL_aba_pointer *obj, CTL_aba_pointer *expected, CTL_aba_pointer desired, memory_order order)
{
    uint16_t count = ((desired & __CTL_ABA_COUNT_MASK) >> 48) + 1;
    uint64_t new_ptr = ((uint64_t)count << 48) + (desired & __CTL_ABA_POINTER_MASK);

    return atomic_compare_exchange_strong_explicit(obj, expected, new_ptr, order, order);
}

static inline CTL_aba_pointer CTL_aba_pointer_atomic_fetch_add(_Atomic CTL_aba_pointer *obj, ptrdiff_t arg, memory_order order)
{
    uint64_t des = (1ull << 48) + (arg & __CTL_ABA_POINTER_MASK);

    return atomic_fetch_add_explicit(obj, des, order);
}

static inline CTL_aba_pointer CTL_aba_pointer_atomic_fetch_sub(_Atomic CTL_aba_pointer *obj, ptrdiff_t arg, memory_order order)
{
    uint64_t des = (1ull << 48) + (arg & __CTL_ABA_POINTER_MASK);

    return atomic_fetch_sub_explicit(obj, des, order);
}
#endif