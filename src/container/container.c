#include "CTL/container.h"

static CTL_functions functions_array = {
    .front = (void *(*)(const CTL_container *))CTL_array_front,
    .back = (void *(*)(const CTL_container *))CTL_array_back,

    .begin = (void (*)(const CTL_container *, CTL_iterator *))CTL_array_begin,
    .end = (void (*)(const CTL_container *, CTL_iterator *))CTL_array_end,

    .at = (void *(*)(const CTL_container *, const size_t))CTL_array_at,
    .iterator_move = (void (*)(const CTL_iterator *, const ptrdiff_t, CTL_iterator *))CTL_array_iterator_move,
    .iterator_equal = (bool (*)(const CTL_iterator *, const CTL_iterator *))CTL_array_iterator_equal,
    .iterator_less = (bool (*)(const CTL_iterator *, const CTL_iterator *))CTL_array_iterator_less,
    .iterator_diff = (ptrdiff_t(*)(const CTL_iterator *, const CTL_iterator *))CTL_array_iterator_diff,

    .size = (size_t(*)(const CTL_container *))CTL_array_size,
    .empty = (bool (*)(const CTL_container *))CTL_array_empty,
};

CTL_functions *CTL_array_functions()
{
    return &functions_array;
}

static CTL_functions functions_vector = {
    .__new = (void (*)(CTL_container *, const size_t, const size_t))CTL_vector_new,
    .__delete = (void (*)(CTL_container *))CTL_vector_delete,

    .front = (void *(*)(const CTL_container *))CTL_vector_front,
    .back = (void *(*)(const CTL_container *))CTL_vector_back,

    .begin = (void (*)(const CTL_container *, CTL_iterator *))CTL_vector_begin,
    .end = (void (*)(const CTL_container *, CTL_iterator *))CTL_vector_end,

    .push_back = (void (*)(CTL_container *, const void *))CTL_vector_push_back,
    .pop_back = (void (*)(CTL_container *))CTL_vector_pop_back,

    .at = (void *(*)(const CTL_container *, const size_t))CTL_vector_at,
    .iterator_move = (void (*)(const CTL_iterator *, const ptrdiff_t, CTL_iterator *))CTL_vector_iterator_move,
    .iterator_equal = (bool (*)(const CTL_iterator *, const CTL_iterator *))CTL_vector_iterator_equal,
    .iterator_less = (bool (*)(const CTL_iterator *, const CTL_iterator *))CTL_vector_iterator_less,
    .iterator_diff = (ptrdiff_t(*)(const CTL_iterator *, const CTL_iterator *))CTL_vector_iterator_diff,

    .size = (size_t(*)(const CTL_container *))CTL_vector_size,
    .empty = (bool (*)(const CTL_container *))CTL_vector_empty,
};

CTL_functions *CTL_vector_functions()
{
    return &functions_vector;
}

static void __CTL_list_new(CTL_container *handle, size_t buf_size, size_t T_size)
{
    CTL_list_new(&handle->list, T_size);
}

static CTL_functions functions_list = {
    .__new = (void (*)(CTL_container *, const size_t, const size_t))__CTL_list_new,
    .__delete = (void (*)(CTL_container *))CTL_list_delete,

    .front = (void *(*)(const CTL_container *))CTL_list_front,
    .back = (void *(*)(const CTL_container *))CTL_list_back,

    .begin = (void (*)(const CTL_container *, CTL_iterator *))CTL_list_begin,
    .end = (void (*)(const CTL_container *, CTL_iterator *))CTL_list_end,

    .push_back = (void (*)(CTL_container *, const void *))CTL_list_push_back,
    .push_front = (void (*)(CTL_container *, const void *))CTL_list_push_front,

    .pop_back = (void (*)(CTL_container *))CTL_list_pop_back,
    .pop_front = (void (*)(CTL_container *))CTL_list_pop_front,

    .iterator_move = (void (*)(const CTL_iterator *, const ptrdiff_t, CTL_iterator *))CTL_list_iterator_move,
    .iterator_equal = (bool (*)(const CTL_iterator *, const CTL_iterator *))CTL_list_iterator_equal,

    .size = (size_t(*)(const CTL_container *))CTL_list_size,
    .empty = (bool (*)(const CTL_container *))CTL_list_empty,
};

CTL_functions *CTL_list_functions()
{
    return &functions_list;
}

static CTL_functions functions_deque = {
    .__new = (void (*)(CTL_container *, const size_t, const size_t))CTL_deque_new,
    .__delete = (void (*)(CTL_container *))CTL_deque_delete,

    .front = (void *(*)(const CTL_container *))CTL_deque_front,
    .back = (void *(*)(const CTL_container *))CTL_deque_back,

    .begin = (void (*)(const CTL_container *, CTL_iterator *))CTL_deque_begin,
    .end = (void (*)(const CTL_container *, CTL_iterator *))CTL_deque_end,

    .push_back = (void (*)(CTL_container *, const void *))CTL_deque_push_back,
    .push_front = (void (*)(CTL_container *, const void *))CTL_deque_push_front,

    .pop_back = (void (*)(CTL_container *))CTL_deque_pop_back,
    .pop_front = (void (*)(CTL_container *))CTL_deque_pop_front,

    .at = (void *(*)(const CTL_container *, const size_t))CTL_deque_at,
    .iterator_move = (void (*)(const CTL_iterator *, const ptrdiff_t, CTL_iterator *))CTL_deque_iterator_move,
    .iterator_equal = (bool (*)(const CTL_iterator *, const CTL_iterator *))CTL_deque_iterator_equal,
    .iterator_less = (bool (*)(const CTL_iterator *, const CTL_iterator *))CTL_deque_iterator_less,
    .iterator_diff = (ptrdiff_t(*)(const CTL_iterator *, const CTL_iterator *))CTL_deque_iterator_diff,

    .size = (size_t(*)(const CTL_container *))CTL_deque_size,
    .empty = (bool (*)(const CTL_container *))CTL_deque_empty,
};

CTL_functions *CTL_deque_functions()
{
    return &functions_deque;
}