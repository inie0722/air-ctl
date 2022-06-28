#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "CTL/allocator.h"

static void (*CTL_malloc_handler)() = NULL; //内存分配失败 处理函数 由用户自定义

void CTL_set_malloc_handler(void (*handler)())
{
    CTL_malloc_handler = handler;
}

//一级分配器
static void *CTL_malloc(size_t size);
static void *CTL_remalloc(void *old_ptr, size_t size);
static void CTL_free(void *ptr);

static void *CTL_malloc(size_t size)
{
    void *result = malloc(size);

    while (!result) // oom处理
    {
        assert(CTL_malloc_handler); //没有自定义处理 直接断言
        CTL_malloc_handler();
        result = malloc(size);
    }
    return result;
}

static void *CTL_remalloc(void *old_ptr, size_t size)
{
    void *result = realloc(old_ptr, size);

    while (!result) // oom处理
    {
        assert(CTL_malloc_handler); //没有自定义处理 直接断言
        CTL_malloc_handler();
        result = realloc(old_ptr, size);
    }
    return result;
}

static void CTL_free(void *ptr)
{
    free(ptr);
}

void *CTL_allocate(size_t size)
{
    void *old_ptr = CTL_malloc(size);
    return old_ptr;
}

void *CTL_reallocate(void *old_ptr, size_t old_size, size_t new_size)
{
    return CTL_remalloc(old_ptr, old_size);
}

void CTL_deallocate(void *ptr, size_t size)
{
    CTL_free(ptr);
}