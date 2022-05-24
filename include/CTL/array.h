/**
 * @file array.h
 * @author 然Y (inie0722@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief array iterator
 *
 */
typedef struct
{
    char *data;
    size_t T_size;
} CTL_array_iterator;

/**
 * @brief array
 *
 */
typedef struct
{
    CTL_array_iterator begin;
    CTL_array_iterator end;
} CTL_array;

/**
 * @brief init array
 *
 * @param handle
 * @param begin array begin address
 * @param end array end address
 * @param T_size element type size
 */
extern void CTL_array_init(CTL_array *handle, const void *begin, const void *end, size_t T_size);

/**
 * @brief get first element
 *
 * @param handle
 * @return void* first element address
 */
extern void *CTL_array_front(const CTL_array *handle);

/**
 * @brief get last element
 *
 * @param handle
 * @return void* last element address
 */
extern void *CTL_array_back(const CTL_array *handle);

/**
 * @brief get begin iterator
 *
 * @param handle
 * @param res result begin iterator
 */
extern void CTL_array_begin(const CTL_array *handle, CTL_array_iterator *res);

/**
 * @brief get end iterator
 *
 * @param handle
 * @param res result end iterator
 */
extern void CTL_array_end(const CTL_array *handle, CTL_array_iterator *res);

/**
 * @brief get index element
 *
 * @param handle
 * @param index
 * @return void* element address
 */
extern void *CTL_array_at(const CTL_array *handle, const size_t index);

/**
 * @brief get the number of elements
 *
 * @param handle
 * @return number of elements
 */
extern size_t CTL_array_size(const CTL_array *handle);

/**
 * @brief get the number of elements that can be held in currently allocated storage
 *
 * @param handle
 * @return size_t number of elements that can be held in currently allocated storage
 */
extern size_t CTL_array_capacity(const CTL_array *handle);

/**
 * @brief if array is empty return true, else return false
 *
 * @param handle
 * @return true
 * @return false
 */
extern bool CTL_array_empty(const CTL_array *handle);

/**
 * @brief iterator move to n
 *
 * @param handle
 * @param n positive move forward, negative move backward
 * @param res result iterator
 */
extern void CTL_array_iterator_move(const CTL_array_iterator *handle, ptrdiff_t n, CTL_array_iterator *res);

/**
 * @brief if left iterator equal right iterator return true, else return false
 *
 * @param left
 * @param right
 * @return true
 * @return false
 */
extern bool CTL_array_iterator_equal(const CTL_array_iterator *left, const CTL_array_iterator *right);

/**
 * @brief if left iterator precedes right iterator return true, else return false
 *
 * @param left
 * @param right
 * @return true
 * @return false
 */
extern bool CTL_array_iterator_less(const CTL_array_iterator *left, const CTL_array_iterator *right);

/**
 * @brief calculate distance between left and right iterator
 *
 * @param left
 * @param right
 * @return ptrdiff_t
 */
extern ptrdiff_t CTL_array_iterator_diff(const CTL_array_iterator *left, const CTL_array_iterator *right);