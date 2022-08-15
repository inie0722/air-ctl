#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "air/ctl/malloc.h"

static void (*CTL_malloc_handler)() = NULL; //内存分配失败 处理函数 由用户自定义

void CTL_set_malloc_handler(void (*handler)())
{
    CTL_malloc_handler = handler;
}

//一级分配器

void *CTL_malloc(size_t size)
{
    void *result = malloc(size);

    while (!result) // oom处理
    {
        //没有自定义处理 直接abort
        if (CTL_malloc_handler == NULL)
            abort();

        CTL_malloc_handler();
        result = malloc(size);
    }
    return result;
}

void *CTL_remalloc(void *old_ptr, size_t size, size_t new_size)
{
    void *result = realloc(old_ptr, size);

    while (!result) // oom处理
    {

        if (CTL_malloc_handler == NULL)
            abort();

        CTL_malloc_handler();
        result = realloc(old_ptr, size);
    }
    return result;
}

void CTL_free(void *ptr, size_t size)
{
    free(ptr);
}