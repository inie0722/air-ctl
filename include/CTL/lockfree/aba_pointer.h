#pragma once

#include <stdint.h>

#include "CTL/lockfree/atomic.h"

#define __CTL_ABA_COUNT_MASK (0XFFFFull << 48)
#define __CTL_ABA_POINTER_MASK (~__CTL_ABA_COUNT_MASK)

typedef uint64_t CTL_aba_pointer;

inline void *CTL_aba_pointer_get(CTL_aba_pointer ptr)
{
    return (void *)(ptr & __CTL_ABA_POINTER_MASK);
}

#if defined _MSC_VER && defined __STDC_NO_ATOMICS__ && __STDC_VERSION__ >= 201112L
inline CTL_aba_pointer CTL_aba_pointer_atomic_load(const _Atomic CTL_aba_pointer *obj, memory_order order = memory_order_seq_cst)
{
    return atomic_load_explicit(obj, order);
}

inline void CTL_aba_pointer_atomic_store(_Atomic CTL_aba_pointer *obj, CTL_aba_pointer desired, memory_order order = memory_order_seq_cst)
{
    uint16_t count = ((desired & __CTL_ABA_COUNT_MASK) >> 48) + 1;
    uint64_t new_ptr = ((uint64_t)count << 48) + (desired & __CTL_ABA_POINTER_MASK);

    atomic_store_explicit(obj, new_ptr, order);
}

inline CTL_aba_pointer CTL_aba_pointer_atomic_exchange(_Atomic CTL_aba_pointer *obj, CTL_aba_pointer desired, memory_order order = memory_order_seq_cst)
{
    uint16_t count = ((desired & __CTL_ABA_COUNT_MASK) >> 48) + 1;
    uint64_t new_ptr = ((uint64_t)count << 48) + (desired & __CTL_ABA_POINTER_MASK);

    return atomic_exchange_explicit(obj, new_ptr, order);
}

inline bool CTL_aba_pointer_atomic_weak(_Atomic CTL_aba_pointer *obj, CTL_aba_pointer *expected, CTL_aba_pointer desired, memory_order order = memory_order_seq_cst)
{
    uint16_t count = ((desired & __CTL_ABA_COUNT_MASK) >> 48) + 1;
    uint64_t new_ptr = ((uint64_t)count << 48) + (desired & __CTL_ABA_POINTER_MASK);

    return atomic_compare_exchange_weak_explicit(obj, &expected, new_ptr, order);
}

inline bool CTL_aba_pointer_atomic_strong(_Atomic CTL_aba_pointer *obj, CTL_aba_pointer *expected, CTL_aba_pointer desired, memory_order order = memory_order_seq_cst)
{
    uint16_t count = ((desired & __CTL_ABA_COUNT_MASK) >> 48) + 1;
    uint64_t new_ptr = ((uint64_t)count << 48) + (desired & __CTL_ABA_POINTER_MASK);

    return atomic_compare_exchange_strong_explicit(obj, &expected, new_ptr, order);
}

inline CTL_aba_pointer CTL_aba_pointer_atomic_fetch_add(_Atomic CTL_aba_pointer *obj, ptrdiff_t arg, memory_order order = memory_order_seq_cst)
{
    uint64_t des = (1ull << 48) + (arg & __CTL_ABA_POINTER_MASK);

    return atomic_fetch_add_explicit(obj, des, order);
}

inline CTL_aba_pointer CTL_aba_pointer_atomic_fetch_sub(_Atomic CTL_aba_pointer *obj, ptrdiff_t arg, memory_order order = memory_order_seq_cst)
{
    uint64_t des = (1ull << 48) + (arg & __CTL_ABA_POINTER_MASK);

    return atomic_fetch_sub_explicit(obj, des, order);
}
#endif