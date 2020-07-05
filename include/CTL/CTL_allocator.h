#pragma once

#include <stddef.h>

extern size_t CTL_get_mem_size();

extern void CTL_set_malloc_handler(void (*handler)()); //内存分配失败 处理函数 由用户自定义

extern void *CTL_allocate(size_t size); //申请内存

extern void *CTL_reallocate(void *old_ptr, size_t old_size, size_t new_size); //重新分配内存

extern void CTL_deallocate(void *ptr, size_t size); //释放内存
