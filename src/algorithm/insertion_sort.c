#include "CTL/algorithm/insertion_sort.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32)
#include <malloc.h>
#endif

static inline void __insertion_sort(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *));
static inline void linear_insert(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *));

void CTL_insertion_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    __insertion_sort((char *)first, (char *)last, T_size, compare);
}

static inline void __insertion_sort(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    for (char *i = first + T_size; i < last; i += T_size)
    {
        linear_insert(first, i, T_size, compare);
    }
}

static inline void linear_insert(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *))
{
#if defined(__linux__) || defined(__APPLE__)
    void *value = alloca(T_size);
#elif defined(_WIN32)
    void *value = _malloca(T_size);
#endif
    memcpy(value, last, T_size);

    //如果尾元素小于头元素 直接插入到 头元素前面就行
    if (compare(value, first))
    {
        memmove(first + T_size, first, last - first);
        memcpy(first, value, T_size);
    }
    else // 尾不小于头
        __CTL_unguarded_linear_insert(last, value, T_size, compare);
}

void __CTL_unguarded_linear_insert(char *last, char *value, size_t T_size, bool (*compare)(const void *, const void *))
{
    char *next = last - T_size;

    while (compare(value, next))
    { // 逆转对(inversion)存在
        memcpy(last, next, T_size);
        last = next;    // 调整迭代器
        next -= T_size; // 左移一个位置
    }
    memcpy(last, value, T_size); // value 的正确落脚处
}