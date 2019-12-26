#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    char *data;  //当前所指的位置
    char *first; //此迭代器 所指缓存区 的头地址
    char *last;  //此迭代器 所指缓存区 的尾地址(含备用空间)
    char **node; //指向map 管控中心
    ptrdiff_t T_size;
} CTL_deque_iterator;

typedef struct
{
    char **map;
    size_t map_size;
    size_t buf_size;
    size_t size;
    CTL_deque_iterator begin; //第一个指针
    CTL_deque_iterator end;   //最后一个指针
} CTL_deque;

void CTL_deque_new(CTL_deque *handle, size_t buf_size, size_t T_size);

void CTL_deque_clear(CTL_deque *handle);

void CTL_deque_delete(CTL_deque *handle);

void *CTL_deque_front(const CTL_deque *handle);

void *CTL_deque_back(const CTL_deque *handle);

CTL_deque_iterator CTL_deque_begin(const CTL_deque *handle);

CTL_deque_iterator CTL_deque_end(const CTL_deque *handle);

void CTL_deque_push_back(CTL_deque *handle, void *data);

void CTL_deque_push_front(CTL_deque *handle, void *data);

CTL_deque_iterator CTL_deque_iterator_move(const CTL_deque_iterator *handle, const ptrdiff_t pos);

void CTL_deque_pop_back(CTL_deque *handle);

void CTL_deque_pop_front(CTL_deque *handle);

CTL_deque_iterator CTL_deque_at(const CTL_deque *handle, size_t pos);

void CTL_deque_insert(CTL_deque *handle, const CTL_deque_iterator *iterator, void *data);

void CTL_deque_erase(CTL_deque *handle, const CTL_deque_iterator *iterator);

bool CTL_deque_iterator_equal(const CTL_deque_iterator *left, const CTL_deque_iterator *right);

bool CTL_deque_iterator_more(const CTL_deque_iterator *left, const CTL_deque_iterator *right);

ptrdiff_t CTL_deque_iterator_diff(const CTL_deque_iterator *left, const CTL_deque_iterator *right);

size_t CTL_deque_size(const CTL_deque *handle);

bool CTL_deque_empty(const CTL_deque *handle);