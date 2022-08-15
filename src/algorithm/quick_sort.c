#include "air/ctl/algorithm/quick_sort.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32)
#include <malloc.h>
#endif

static inline void __quick_sort(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    if (last - first < T_size * 2)
        return;

#if defined(__linux__) || defined(__APPLE__)
    void *value = alloca(T_size);
#elif defined(_WIN32)
    void *value = _malloca(T_size);
#endif

    void *med[3];
    if (last - first > T_size * 16)
    {
        size_t d = (last - first) / T_size / 8 * T_size;

        med[0] = __CTL_median(first, first + d, first + d * 2, compare);
        med[1] = __CTL_median(first + d * 3, first + d * 4, first + d * 5, compare);
        med[2] = __CTL_median(first + d * 6, first + d * 7, last - T_size, compare);
        med[0] = __CTL_median(med[0], med[1], med[2], compare);
    }
    else
    {
        med[0] = __CTL_median(first, first + ((last - first) / T_size / 2 * T_size), last - T_size, compare);
    }
    memcpy(value, med[0], T_size);

    void *cur = __CTL_unguarded_partition(first, last, value, T_size, compare);

    __quick_sort(first, cur, T_size, compare);
    __quick_sort(cur, last, T_size, compare);
}

void CTL_quick_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    __quick_sort(first, last, T_size, compare);
}

void *__CTL_median(char *a, char *b, char *c, bool (*compare)(const void *, const void *))
{
    if (compare(b, a))
    {
        if (compare(a, c))
            return a;
        else if (compare(c, b))
            return b;
        else
            return c;
    }
    else if (compare(c, a))
        return a;
    else if (compare(c, b))
        return c;
    else
        return b;
}

char *__CTL_unguarded_partition(char *first, char *last, char *value, size_t T_size, bool (*compare)(const void *, const void *))
{
#if defined(__linux__) || defined(__APPLE__)
    void *tmp = alloca(T_size);
#elif defined(_WIN32)
    void *tmp = _malloca(T_size);
#endif

    while (1)
    {
        while (compare(first, value))
            first += T_size;
        last -= T_size; //调整last 不进行调整的话last等于旧值
        while (compare(value, last))
            last -= T_size;

        if (first >= last)
            return first;

        memcpy(tmp, first, T_size);
        memcpy(first, last, T_size);
        memcpy(last, tmp, T_size);

        first += T_size; //调整first 原因同上
    }
}