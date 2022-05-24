/**
 * @file container.h
 * @author 然Y (inie0722@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "CTL/array.h"
#include "CTL/vector.h"
#include "CTL/list.h"
#include "CTL/deque.h"

/**
 * @brief container
 *
 */
typedef union
{
    CTL_array array;
    CTL_vector vector;
    CTL_list list;
    CTL_deque deque;
} CTL_container;

/**
 * @brief iterator
 *
 */
typedef union
{
    struct
    {
        char *data;
        size_t T_size;
    };

    CTL_array_iterator array;
    CTL_vector_iterator vector;
    CTL_list_iterator list;
    CTL_deque_iterator deque;
} CTL_iterator;

/**
 * @brief member function
 *
 */
typedef struct
{
    /// @brief create a container
    void (*__new)(CTL_container *handle, size_t data_size, size_t T_size);

    /// @brief destroy container
    void (*__delete)(CTL_container *handle);

    /// @brief get first element
    void *(*front)(const CTL_container *handle);

    /// @brief get last element
    void *(*back)(const CTL_container *handle);

    /// @brief get begin iterator
    void (*begin)(const CTL_container *handle, CTL_iterator *res);

    /// @brief get end iterator
    void (*end)(const CTL_container *handle, CTL_iterator *res);

    /// @brief adds an element to the end
    void (*push_back)(CTL_container *handle, const void *element);

    /// @brief adds an element to the front
    void (*push_front)(CTL_container *handle, const void *element);

    /// @brief erases the last element
    void (*pop_back)(CTL_container *handle);

    /// @brief erases the first element
    void (*pop_front)(CTL_container *handle);

    /// @brief inserts an element at a given position
    void (*insert)(CTL_container *handle, CTL_iterator *iterator, const void *element);

    /// @brief  erases the element at a given position
    void (*erase)(CTL_container *handle, CTL_iterator *iterator);

    /// @brief get index element
    void *(*at)(const CTL_container *handle, size_t index);

    /// @brief iterator move to n
    void (*iterator_move)(const CTL_iterator *handle, const ptrdiff_t n, CTL_iterator *res);

    /// @brief if left iterator equal right iterator return true, else return false
    bool (*iterator_equal)(const CTL_iterator *left, const CTL_iterator *right);

    /// @brief if left iterator precedes right iterator return true, else return false
    bool (*iterator_less)(const CTL_iterator *left, const CTL_iterator *right);

    /// @brief calculate distance between left and right iterator
    ptrdiff_t (*iterator_diff)(const CTL_iterator *left, const CTL_iterator *right);

    /// @brief get the number of elements that can be held in currently allocated storage
    size_t (*size)(const CTL_container *handle);

    /// @brief if vector is empty return true, else return false
    bool (*empty)(const CTL_container *handle);
} CTL_functions;

/**
 * @brief get array member function
 *
 * @return CTL_functions*
 */
CTL_functions *CTL_array_functions();

/**
 * @brief get vector member function
 *
 * @return CTL_functions*
 */
CTL_functions *CTL_vector_functions();

/**
 * @brief get list member function
 *
 * @return CTL_functions*
 */
CTL_functions *CTL_list_functions();

/**
 * @brief get deque member function
 *
 * @return CTL_functions*
 */
CTL_functions *CTL_deque_functions();