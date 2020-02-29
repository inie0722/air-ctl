#include <stdbool.h>
#include <stddef.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#include "CTL_allocator.h"

size_t CTL_mem_size = 0;

void (*CTL_malloc_handler)() = NULL; //内存分配失败 处理函数 由用户自定义

size_t CTL_get_mem_size()
{
    return CTL_mem_size;
}

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

    while (!result) //oom处理
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

    while (!result) //oom处理
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

#ifdef CTL_NO_ALLOCATE
void *CTL_allocate(size_t size)
{
    void *old_ptr = CTL_malloc(size);
    CTL_mem_size += size;
    return old_ptr;
}

void *CTL_reallocate(void *old_ptr, size_t old_size, size_t new_size)
{
    CTL_mem_size += (new_size - old_size);
    return CTL_remalloc(old_ptr, old_size);
}

void CTL_deallocate(void *ptr, size_t size)
{
    CTL_mem_size -= size;
    CTL_free(ptr);
}
#else
#include <threads.h>

//二级分配器
#define ALIGN 8                      //区块上调边界
#define MAX_BYTES 128                //区块最大上限
#define NFREELISTS MAX_BYTES / ALIGN //free list 数量

#define ROUND_UP(bytes) (((bytes) + ALIGN - 1) & ~(ALIGN - 1))      //将bytes 上调至ALIGN 的倍数
#define FREELIST_INDEX(bytes) ((((bytes) + ALIGN - 1) / ALIGN) - 1) //根据bytes大小 决定free list

typedef union obj //free list 节点
{
    union obj *free_list_link; //指向下一个节点
    char client_data[1];       //指向内存块
} obj;
//这里先看一下 obj联合体 书上说此联合体为 一物两用 同时指向下一个节点 和 内存块
//其实可以理解为 一个语法糖 这里client_data 指向内存块
//free_list_link 也指向内存块 也就是 下一个节点的地址 存放在内存块中

static void *refill(size_t size); //free list 填充函数

static void *chunk_alloc(size_t size, size_t *nobjs); //该函数 负责向内存池 索要 内存

static obj *free_list[NFREELISTS] = {NULL};

//这三个变量 负责管理内存池
static char *begin_free = 0; // 内存池的首地址
static char *end_free = 0;   //内存池的结束地址
static size_t heap_size = 0; //大小
static mtx_t mutex;          //内存池互斥锁

void *CTL_allocate(size_t size)
{
    CTL_mem_size += size;

    obj *result;

    if (size > MAX_BYTES)
    {
        //申请内存 大于MAX_BYTES 直接交给 一级分配器
        return CTL_malloc(size);
    }

    static bool mutex_status = false;
    if (mutex_status == false)
    {
        mtx_init(&mutex, mtx_plain);
        mutex_status = true;
    }

    mtx_lock(&mutex);
    //找到合适的 free list
    obj **my_free_list = free_list + FREELIST_INDEX(size);
    result = *my_free_list;

    //如果 没用可以内存块 就重新填充 free list
    if (!result)
    {
        result = (obj *)refill(ROUND_UP(size));
    }
    else
    {
        *my_free_list = result->free_list_link;
    }

    mtx_unlock(&mutex);
    return result;
}

void *CTL_reallocate(void *old_ptr, size_t old_size, size_t new_size)
{
    //申请和释放 都大于 MAX_BYTES 交给 一级分配器处理
    if (old_size > MAX_BYTES && new_size > MAX_BYTES)
    {
        return CTL_remalloc(old_ptr, new_size);
    }
    else
    {
        void *new_ptr = CTL_allocate(new_size);
        memcpy(new_ptr, old_ptr, new_size > old_size ? new_size : old_size);
        CTL_deallocate(old_ptr, old_size);
        return new_ptr;
    }
}

void CTL_deallocate(void *ptr, size_t size)
{
    CTL_mem_size -= size;
    //释放内存 大于 MAX_BYTES 直接 交给 一级分配器
    if (size > MAX_BYTES)
    {
        CTL_free(ptr);
    }
    else
    {
        mtx_lock(&mutex);
        //将其交给 合适的 free list
        obj **my_free_list = free_list + FREELIST_INDEX(size);
        ((obj *)ptr)->free_list_link = *my_free_list;
        *my_free_list = (obj *)ptr;
    }
    mtx_unlock(&mutex);
}

static void *refill(size_t size)
{
    obj *result;
    //申请 20个 size 大小区块 具体过程由chunk_alloc 函数执行
    size_t nobjs = 20;
    char *chunk = (char *)chunk_alloc(size, &nobjs);

    //如果只获得了一个区块 就将这个区块直接返回给用户 此时free_list中无新节点
    if (nobjs == 1)
    {
        return (chunk);
    }
    //否则就准备 把新区块 加入 free list
    obj **my_free_list = free_list + FREELIST_INDEX(size);
    //第一个区块块 返回给用户
    result = (obj *)chunk;
    obj *next_obj = *my_free_list = (obj *)(chunk + size);

    //挂载区块操作
    for (size_t i = 1;; ++i)
    {
        //循环从1开始，因为第一区块已经返回给用户了
        obj *current_obj = next_obj;
        next_obj = (obj *)((char *)next_obj + size);

        if (nobjs - 1 == i)
        {
            current_obj->free_list_link = NULL;
            break;
        }
        else
        {
            current_obj->free_list_link = next_obj;
        }
    }
    return result;
}

static void *chunk_alloc(size_t size, size_t *nobjs)
{
    void *result;
    //要分配的 内存 数量
    size_t bytes_total = size * (*nobjs);
    //内存池 剩余空间
    size_t bytes_left = end_free - begin_free;

    //内存池 剩余空间 足够需求
    if (bytes_left >= bytes_total)
    {
        result = begin_free;
        begin_free += bytes_total;
        return result;
    }
    else if (bytes_left > size)
    {
        //内存池 空间不足 但至少够一个区块的
        *nobjs = (size_t)(bytes_left / size);
        bytes_total = size * (*nobjs);
        result = begin_free;
        begin_free += bytes_total;
        return result;
    }
    else
    {
        //如果 一个区块 空间都不足够了
        size_t bytes_to_get = 2 * bytes_total + ROUND_UP(heap_size >> 4);

        //如果 内存池还有剩余 就把它们 放入 free list 中
        if (bytes_left > 0)
        {
            obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
            ((obj *)begin_free)->free_list_link = *my_free_list;
            *my_free_list = (obj *)begin_free;
        }

        begin_free = (char *)malloc(bytes_to_get); //调用malloc 申请新的内存池
        if (!begin_free)
        {
            //如果 内存空间不足
            //去free list看看 还有没有 较大的区块 将它先返回 给用户 使用
            for (size_t i = size; i < MAX_BYTES; i += ALIGN)
            {
                obj **my_free_list = free_list + FREELIST_INDEX(i);
                obj *p = *my_free_list;
                if (p)
                {
                    //free list 中 还有 较大的区块 将它调整出来
                    *my_free_list = p->free_list_link;
                    begin_free = (char *)p;
                    end_free = begin_free + i;
                    //递归 调整free list
                    return chunk_alloc(size, nobjs);
                }
            }
            //如果 彻底 没有空间了 这时候 交给一级分配器 处理
            //看看 OOM机制 是否有效
            //end_free = 0;
            begin_free = (char *)CTL_malloc(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = begin_free + bytes_to_get;
        //递归 调整free list
        return chunk_alloc(size, nobjs);
    }
}

#endif