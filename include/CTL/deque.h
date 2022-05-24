/**
 * @file deque.h
 * @author 然Y (inie0722@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief deque iterator
 *
 */
typedef struct
{
    char *data; //当前所指的位置
    size_t T_size;
    char *first; //此迭代器 所指缓存区 的头地址
    char *last;  //此迭代器 所指缓存区 的尾地址(含备用空间)
    char **node; //指向map 管控中心
} CTL_deque_iterator;

/**
 * @brief deque
 *
 */
typedef struct
{
    char **map;
    size_t map_size;
    size_t data_size;
    size_t size;
    CTL_deque_iterator begin; //第一个指针
    CTL_deque_iterator end;   //最后一个指针
} CTL_deque;

/**
 * @brief create deque
 *
 * @param handle
 * @param data_size initial data size
 * @param T_size element type size
 */
extern void CTL_deque_new(CTL_deque *handle, size_t data_size, size_t T_size);

/**
 * @brief destroy deque
 *
 * @param handle
 */
extern void CTL_deque_delete(CTL_deque *handle);

/**
 * @brief clear deque
 *
 * @param handle
 */
extern void CTL_deque_clear(CTL_deque *handle);

/**
 * @brief get first element
 *
 * @param handle
 * @return void* first element address
 */
extern void *CTL_deque_front(const CTL_deque *handle);

/**
 * @brief get last element
 *
 * @param handle
 * @return void* last element address
 */
extern void *CTL_deque_back(const CTL_deque *handle);

/**
 * @brief get begin iterator
 *
 * @param handle
 * @param res result begin iterator
 */
extern void CTL_deque_begin(const CTL_deque *handle, CTL_deque_iterator *res);

/**
 * @brief get end iterator
 *
 * @param handle
 * @param res result end iterator
 */
extern void CTL_deque_end(const CTL_deque *handle, CTL_deque_iterator *res);

/**
 * @brief adds an element to the end
 *
 * @param handle
 * @param element element address
 */
extern void CTL_deque_push_back(CTL_deque *handle, const void *element);

/**
 * @brief adds an element to the front
 *
 * @param handle
 * @param element element address
 */
extern void CTL_deque_push_front(CTL_deque *handle, const void *element);

/**
 * @brief erases the last element
 *
 * @param handle
 */
extern void CTL_deque_pop_back(CTL_deque *handle);

/**
 * @brief erases the first element
 *
 * @param handle
 */
extern void CTL_deque_pop_front(CTL_deque *handle);

/**
 * @brief inserts an element at a given position
 *
 * @param handle
 * @param iterator insert position
 * @param element element address
 */
extern void CTL_deque_insert(CTL_deque *handle, CTL_deque_iterator *iterator, const void *element);

/**
 * @brief erases the element at a given position
 *
 * @param handle
 * @param iterator erase position
 */
extern void CTL_deque_erase(CTL_deque *handle, CTL_deque_iterator *iterator);

/**
 * @brief get index element
 *
 * @param handle
 * @param index
 * @return void* element address
 */
void *CTL_deque_at(const CTL_deque *handle, const size_t index);

/**
 * @brief get the number of elements
 *
 * @param handle deque handle
 * @return size_t number of elements
 */
size_t CTL_deque_size(const CTL_deque *handle);

/**
 * @brief if deque is empty return true, else return false
 *
 * @param handle deque handle
 * @return true
 * @return false
 */
bool CTL_deque_empty(const CTL_deque *handle);

/**
 * @brief iterator move to n
 *
 * @param handle
 * @param n positive move forward, negative move backward
 * @param res result iterator
 */
void CTL_deque_iterator_move(const CTL_deque_iterator *handle, const ptrdiff_t n, CTL_deque_iterator *res);

/**
 * @brief if left iterator equal right iterator return true, else return false
 *
 * @param left
 * @param right
 * @return true
 * @return false
 */
bool CTL_deque_iterator_equal(const CTL_deque_iterator *left, const CTL_deque_iterator *right);

/**
 * @brief if left iterator precedes right iterator return true, else return false
 *
 * @param left
 * @param right
 * @return true
 * @return false
 */
bool CTL_deque_iterator_less(const CTL_deque_iterator *left, const CTL_deque_iterator *right);

/**
 * @brief calculate distance between left and right iterator
 *
 * @param left
 * @param right
 * @return ptrdiff_t
 */
ptrdiff_t CTL_deque_iterator_diff(const CTL_deque_iterator *left, const CTL_deque_iterator *right);