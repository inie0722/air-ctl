#include "CTL/algorithm/tim_sort.h"

#include "CTL/algorithm/merge_sort.h"
#include "CTL/algorithm/insertion_sort.h"

#include <stdbool.h>
#include <stddef.h>
#include <malloc.h>

#if defined(linux) || defined(__linux) || defined(__linux__)
#include <alloca.h>
#elif defined(__WIN32__) || defined(_WIN32)
#include <malloc.h>
#define alloca(size) _alloca(size)
#endif

#define MIN_RUN 16

static inline size_t next_partition(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));

typedef struct
{
    void *first;
    void *last;
} run;

static inline size_t run_length(run node)
{
    return node.last - node.first;
}

static inline size_t lg(size_t n)
{
    size_t i = 0;
    for (; n > 1; ++i)
    {
        n >>= 1;
    }
    return i;
}

static inline bool merge_rule(run *stack, size_t top)
{
    if (top < 1)
        return false;
    else if (top < 2)
        return run_length(stack[top]) > run_length(stack[top - 1]);
    else
        return run_length(stack[top]) > run_length(stack[top - 1]) ||
               run_length(stack[top]) + run_length(stack[top - 1]) > run_length(stack[top - 2]);
}

void CTL_tim_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    void *buf = malloc(last - first);
    run *stack = alloca(sizeof(run) * lg((last - first) / T_size) * 2);
    size_t top = 0;
    void *cur = first;

    while (cur < last)
    {
        size_t len = next_partition(cur, last, T_size, compare);
        stack[top].first = cur;
        stack[top].last = stack[top].first + len * T_size;
        cur = stack[top].last;

        while (merge_rule(stack, top))
        {
            if (buf != NULL)
                __CTL_merge(stack[top - 1].first, stack[top - 1].last, stack[top].first, stack[top].last, buf, T_size, compare);
            else
                __CTL_inplace_merge(stack[top - 1].first, stack[top - 1].last, stack[top].first, stack[top].last, T_size, compare);

            stack[top - 1].last = stack[top].last;
            --top;
        }
        ++top;
    }

    while (top > 1)
    {
        --top;
        if (buf != NULL)
            __CTL_merge(stack[top - 1].first, stack[top - 1].last, stack[top].first, stack[top].last, buf, T_size, compare);
        else
            __CTL_inplace_merge(stack[top - 1].first, stack[top - 1].last, stack[top].first, stack[top].last, T_size, compare);

        stack[top - 1].last = stack[top].last;
    }

    if (buf != NULL)
        free(buf);
}

static inline size_t next_partition(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    if (last - first < 2 * T_size)
        return last - first;
    size_t run_len = 1;

    void *cur = first;
    void *next = first + T_size;

    if (compare(cur, next))
    {
        while (next < last && compare(cur, next))
        {
            ++run_len;
            cur += T_size;
            next += T_size;
        }
    }
    else
    {
        while (next < last && compare(cur, next))
        {
            ++run_len;
            cur += T_size;
            next += T_size;
        }
        __CTL_reverse(first, next, T_size);
    }

    //run 长度 过小
    if (next < last && run_len < MIN_RUN)
    {
        last = first + MIN_RUN * T_size < last ? first + MIN_RUN * T_size : last;
        CTL_insertion_sort(first, last, T_size, compare);
        return (last - first) / T_size;
    }
    else
    {
        return run_len;
    }
}