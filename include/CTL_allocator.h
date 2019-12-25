#pragma once

#include <malloc.h>

extern int CTL_debug_mem;

extern size_t CTL_debug_mem_size;

extern void (*CTL_malloc_handler)(); //内存分配失败 处理函数 由用户自定义

void *CTL_allocate(size_t size); //申请内存

void *CTL_reallocate(void *old_ptr, size_t old_size, size_t new_size); //重新分配内存

void CTL_deallocate(void *ptr, size_t size); //释放内存
