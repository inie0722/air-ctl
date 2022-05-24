#include "CTL/algorithm/merge_sort.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <malloc.h>
#endif

static void __merge_sort(char *first, char *last, void *buf, size_t T_size, bool (*compare)(const void *, const void *));
static inline void __merge(char *first_1, char *last_1, char *first_2, char *last_2, void *buf, size_t T_size, bool (*compare)(const void *, const void *));
static void __inplace_merge_sort(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *));

void CTL_merge_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    void *buf = malloc((char*)last - (char*)first);
    if (buf != NULL)
    {
        __merge_sort((char *)first, (char *)last, buf, T_size, compare);
        free(buf);
    }
    else
    {
        __inplace_merge_sort((char *)first, (char *)last, T_size, compare);
    }
}

void __CTL_reverse(char *first, char *last, size_t T_size)
{
#if defined(__linux__) || defined(__APPLE__)
    void *tmp = alloca(T_size);
#elif defined(_WIN32)
    void *tmp = _malloca(T_size);
#endif

    for (size_t i = 0; i < (last - first) / T_size / 2; ++i)
    {
        memcpy(tmp, first + (i * T_size), T_size);
        memcpy(first + (i * T_size), last + ((-i - 1) * T_size), T_size);
        memcpy(last + ((-i - 1) * T_size), tmp, T_size);
    }
}

static void __merge_sort(char *first, char *last, void *buf, size_t T_size, bool (*compare)(const void *, const void *))
{
    size_t mid = (last - first) / T_size / 2;
    if (last - first > T_size)
    {
        __merge_sort(first, last - (mid * T_size), buf, T_size, compare);
        __merge_sort(last - (mid * T_size), last, buf, T_size, compare);
        __CTL_merge(first, last - (mid * T_size), last - (mid * T_size), last, buf, T_size, compare);
    }
}

void __CTL_merge(char *first_1, char *last_1, char *first_2, char *last_2, char *buf, size_t T_size, bool (*compare)(const void *, const void *))
{
    char *cur_1 = first_1;
    char *cur_2 = first_2;

    size_t i = 0;
    while (cur_1 < last_1 && cur_2 < last_2)
    {
        if (!compare(cur_1, cur_2))
        {
            memcpy(buf + i, cur_2, T_size);
            cur_2 += T_size;
        }
        else
        {
            memcpy(buf + i, cur_1, T_size);
            cur_1 += T_size;
        }
        i += T_size;
    }

    if (cur_1 < last_1)
    {
        memcpy(buf + i, cur_1, last_1 - cur_1);
        i += last_1 - cur_1;
    }
    else if (cur_2 < last_2)
    {
        memcpy(buf + i, cur_2, last_2 - cur_2);
        i += last_2 - cur_2;
    }

    memcpy(first_1, buf, i);
}

static void __inplace_merge_sort(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    size_t mid = (last - first) / T_size / 2;
    if (last - first > T_size)
    {
        __inplace_merge_sort(first, last - (mid * T_size), T_size, compare);
        __inplace_merge_sort(last - (mid * T_size), last, T_size, compare);
        __CTL_inplace_merge(first, last - (mid * T_size), last - (mid * T_size), last, T_size, compare);
    }
}

void __CTL_inplace_merge(char *first_1, char *last_1, char *first_2, char *last_2, size_t T_size, bool (*compare)(const void *, const void *))
{
    while (first_1 < last_2 && first_2 < last_2)
    {
        char *cur_1 = first_1;
        char *cur_2 = first_2;

        while (cur_1 < last_2 && !compare(cur_2, cur_1))
        {
            cur_1 += T_size;
        }

        if (cur_1 == last_2 || cur_1 > first_2)
            break;

        while (cur_2 < last_2 && compare(cur_2, cur_1))
        {
            cur_2 += T_size;
        }

        __CTL_reverse(cur_1, first_2, T_size);
        __CTL_reverse(first_2, cur_2, T_size);
        __CTL_reverse(cur_1, cur_2, T_size);

        first_1 = cur_1 + (cur_2 - first_2);
        first_2 = cur_2;
    }
}