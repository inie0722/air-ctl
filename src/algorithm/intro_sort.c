#include "air/ctl/algorithm/intro_sort.h"

#include "air/ctl/algorithm/insertion_sort.h"
#include "air/ctl/algorithm/heap_sort.h"
#include "air/ctl/algorithm/quick_sort.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32)
#include <malloc.h>
#endif

#define THRESHOLD 16

static inline void introsort_loop(char *first, char *last, size_t depth_limit, size_t T_size, bool (*compare)(const void *, const void *));

static inline void __intro_sort(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *));

static inline size_t lg(size_t n)
{
    size_t i = 0;
    for (; n > 1; ++i)
    {
        n >>= 1;
    }
    return i;
}

void CTL_intro_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    __intro_sort((char *)first, (char *)last, T_size, compare);
}

void __intro_sort(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *))
{
#if defined(__linux__) || defined(__APPLE__)
    void *value = alloca(T_size);
#elif defined(_WIN32)
    void *value = _malloca(T_size);
#endif
    if (first != last)
    {
        //最多允许递归logn*2
        introsort_loop(first, last, lg((last - first) / T_size * 2), T_size, compare);
        if (last - first > THRESHOLD * T_size)
        {
            //对第一段元素进行排序
            CTL_insertion_sort(first, first + THRESHOLD * T_size, T_size, compare);

            //因为这些元素都被快排过了 第一段元素必定拥有最小的元素 因此直接进入内部循环即可

            int t = 0;
            for (char *i = first + THRESHOLD * T_size; i != last; i += T_size)
            {
                ++t;
                memcpy(value, i, T_size);
                __CTL_unguarded_linear_insert(i, value, T_size, compare);
            }
        }
        else
        {
            CTL_insertion_sort(first, last, T_size, compare);
        }
    }
}

static void introsort_loop(char *first, char *last, size_t depth_limit, size_t T_size, bool (*compare)(const void *, const void *))
{
#if defined(__linux__) || defined(__APPLE__)
    void *value = alloca(T_size);
#elif defined(_WIN32)
    void *value = _malloca(T_size);
#endif
    while (last - first > THRESHOLD * T_size)
    {
        //如果到达递归深度极限
        if (depth_limit == 0)
        {
            //剩余部分直接采用堆排序
            CTL_heap_sort(first, last, T_size, compare);
            break;
        }
        else
        {
            --depth_limit;
            //进行快排分割

            size_t d = (last - first) / T_size / 8 * T_size;

            void *med[3];
            med[0] = __CTL_median(first, first + d, first + d * 2, compare);
            med[1] = __CTL_median(first + d * 3, first + d * 4, first + d * 5, compare);
            med[2] = __CTL_median(first + d * 6, first + d * 7, last - T_size, compare);
            med[0] = __CTL_median(med[0], med[1], med[2], compare);
            memcpy(value, med[0], T_size);

            void *cur = __CTL_unguarded_partition(first, last, value, T_size, compare);

            //对右半段递归处理
            introsort_loop(cur, last, depth_limit, T_size, compare);
            //introsort_loop(first, cur, depth_limit);
            //左半段循环处理
            last = cur;
        }
    }
}