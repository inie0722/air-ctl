/**
 * @file vector.h
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
 * @brief vector iterator
 *
 */
typedef struct
{
    char *data;
    size_t T_size;
} CTL_vector_iterator;

/**
 * @brief vector
 *
 */
typedef struct
{
    CTL_vector_iterator begin;
    CTL_vector_iterator end;
    CTL_vector_iterator end_of_storage;
} CTL_vector;

/**
 * @brief create vector
 *
 * @param handle vector handle
 * @param data_size initial data size
 * @param T_size element type size
 */
extern void CTL_vector_new(CTL_vector *handle, const size_t data_size, const size_t T_size);

/**
 * @brief clear vector
 *
 * @param handle vector handle
 */
extern void CTL_vector_clear(CTL_vector *handle);

/**
 * @brief destroy vector
 *
 * @param handle vector handle
 */
extern void CTL_vector_delete(CTL_vector *handle);

/**
 * @brief get first element
 *
 * @param handle vector handle
 * @return void* first element address
 */
extern void *CTL_vector_front(const CTL_vector *handle);

/**
 * @brief get last element
 *
 * @param handle vector handle
 * @return void* last element address
 */
extern void *CTL_vector_back(const CTL_vector *handle);

/**
 * @brief get begin iterator
 *
 * @param handle vector handle
 * @param res result begin iterator
 */
extern void CTL_vector_begin(const CTL_vector *handle, CTL_vector_iterator *res);

/**
 * @brief get end iterator
 *
 * @param handle vector handle
 * @param res result end iterator
 */
extern void CTL_vector_end(const CTL_vector *handle, CTL_vector_iterator *res);

/**
 * @brief adds an element to the end
 *
 * @param handle vector handle
 * @param data element address
 */
extern void CTL_vector_push_back(CTL_vector *handle, const void *element);

/**
 * @brief erases the last element
 *
 * @param handle vector handle
 */
extern void CTL_vector_pop_back(CTL_vector *handle);

/**
 * @brief inserts an element at a given position
 *
 * @param handle vector handle
 * @param iterator insert position
 * @param data
 */
extern void CTL_vector_insert(CTL_vector *handle, CTL_vector_iterator *iterator, const void *element);

/**
 * @brief erases the element at a given position
 *
 * @param handle vector handle
 * @param iterator erase position
 */
extern void CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator *iterator);

/**
 * @brief get index element
 *
 * @param handle vector handle
 * @param index
 * @return void* element address
 */
extern void *CTL_vector_at(const CTL_vector *handle, const size_t index);

/**
 * @brief get the number of elements
 *
 * @param handle vector handle
 * @return size_t number of elements
 */
extern size_t CTL_vector_size(const CTL_vector *handle);

/**
 * @brief get the number of elements that can be held in currently allocated storage
 *
 * @param handle
 * @return size_t number of elements that can be held in currently allocated storage
 */
extern size_t CTL_vector_capacity(const CTL_vector *handle);

/**
 * @brief if vector is empty return true, else return false
 *
 * @param handle vector handle
 * @return true
 * @return false
 */
extern bool CTL_vector_empty(const CTL_vector *handle);

/**
 * @brief iterator move to n
 *
 * @param handle
 * @param n
 * @param res result iterator
 */
extern void CTL_vector_iterator_move(const CTL_vector_iterator *handle, const ptrdiff_t n, CTL_vector_iterator *res);

/**
 * @brief if left iterator equal right iterator return true, else return false
 *
 * @param left
 * @param right
 * @return true
 * @return false
 */
extern bool CTL_vector_iterator_equal(const CTL_vector_iterator *left, const CTL_vector_iterator *right);

/**
 * @brief if left iterator precedes right iterator return true, else return false
 *
 * @param left
 * @param right
 * @return true
 * @return false
 */
extern bool CTL_vector_iterator_less(const CTL_vector_iterator *left, const CTL_vector_iterator *right);

/**
 * @brief calculate distance between left and right iterator
 *
 * @param left
 * @param right
 * @return ptrdiff_t
 */
extern ptrdiff_t CTL_vector_iterator_diff(const CTL_vector_iterator *left, const CTL_vector_iterator *right);