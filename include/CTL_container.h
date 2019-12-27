#pragma once

#include "CTL_array.h"
#include "CTL_vector.h"
#include "CTL_list.h"
#include "CTL_deque.h"

typedef union {
    CTL_array array;
    CTL_vector vector;
    CTL_list list;
    CTL_deque deque;
} CTL_container;

typedef union {
    CTL_array_iterator array;
    CTL_vector_iterator vector;
    CTL_list_iterator list;
    CTL_deque_iterator deque;
} CTL_iterator;

typedef struct
{
    void (*__new)(CTL_container *handle, size_t buf_size, size_t T_size);
    //void (*clear)(CTL_container *handle);
    void (*__delete)(CTL_container *handle);
    void *(*front)(const CTL_container *handle);
    void *(*back)(const CTL_container *handle);
    CTL_iterator (*begin)(const CTL_container *handle);
    CTL_iterator (*end)(const CTL_container *handle);
    void (*push_back)(CTL_container *handle, void *data);
    void (*push_front)(CTL_container *handle, void *data);
    void (*pop_back)(CTL_container *handle);
    void (*pop_front)(CTL_container *handle);
    void (*insert)(CTL_container *handle, const CTL_iterator *iterator, void *data);
    void (*erase)(CTL_container *handle, const CTL_iterator *iterator);
    CTL_iterator (*at)(const CTL_container *handle, size_t pos);
    CTL_iterator (*iterator_move)(const CTL_container *handle, const ptrdiff_t pos);
    bool (*iterator_equal)(const CTL_iterator *left, const CTL_iterator *right);
    bool (*iterator_more)(const CTL_iterator *left, const CTL_iterator *right);
    ptrdiff_t (*iterator_diff)(const CTL_iterator *left, const CTL_iterator *right);
    size_t (*size)(const CTL_container *handle);
    //size_t (*capacity)(const CTL_container *handle);
    bool (*empty)(const CTL_container *handle);
} CTL_functions;

CTL_functions *CTL_array_functions();

CTL_functions *CTL_vector_functions();

CTL_functions *CTL_list_functions();

CTL_functions *CTL_deque_functions();