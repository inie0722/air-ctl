#include <stddef.h>
#include <string.h>

#include "CTL_vector.h"
#include "CTL_allocator.h"

static void insert_aux(CTL_vector *handle, CTL_vector_iterator *iterator, size_t n);

void CTL_vector_new(CTL_vector *handle, const size_t buf_size, const size_t T_size)
{
    handle->begin.T_size = T_size;
    handle->end.T_size = T_size;
    handle->end_of_storage.T_size = T_size;

    handle->begin.data = (char *)CTL_allocate(T_size * buf_size);
    handle->end.data = handle->begin.data;
    handle->end_of_storage.data = handle->begin.data + (T_size * buf_size);
}

void CTL_vector_clear(CTL_vector *handle)
{
    handle->end.data = handle->begin.data;
}

void CTL_vector_delete(CTL_vector *handle)
{
    size_t size = handle->begin.T_size * CTL_vector_iterator_diff(&handle->end_of_storage, &handle->begin);
    CTL_deallocate(handle->begin.data, size);
}

void *CTL_vector_front(const CTL_vector *handle)
{
    return handle->begin.data;
}

void *CTL_vector_back(const CTL_vector *handle)
{
    return handle->end.data - handle->end.T_size;
}

void CTL_vector_begin(const CTL_vector *handle, CTL_vector_iterator *ret)
{
    *ret = handle->begin;
}

void CTL_vector_end(const CTL_vector *handle, CTL_vector_iterator *ret)
{
    *ret = handle->end;
}

void CTL_vector_push_back(CTL_vector *handle, const void *data)
{
    size_t size = handle->end.T_size * CTL_vector_size(handle);
    size_t capacity = handle->end.T_size * CTL_vector_capacity(handle);
    //空间 不足 扩充一倍
    if (size >= capacity)
    {
        insert_aux(handle, &handle->end, 1);
    }

    memcpy(handle->end.data, data, handle->end.T_size);
    handle->end.data += handle->end.T_size;
}

void CTL_vector_pop_back(CTL_vector *handle)
{
    handle->end.data -= handle->end.T_size;
}

static void insert_aux(CTL_vector *handle, CTL_vector_iterator *iterator, size_t n)
{
    size_t size = handle->end.T_size * CTL_vector_size(handle);
    char *ptr = (char *)CTL_allocate(2 * size);
    //拷贝插入点前面的数据
    memcpy(ptr, handle->begin.data, iterator->data - handle->begin.data);

    //拷贝插入点后面的数据 并空出一个位置
    memcpy(ptr + (iterator->data - handle->begin.data) + (handle->end.T_size * n), iterator->data, handle->end.data - iterator->data);

    CTL_deallocate(handle->begin.data, size);

    handle->begin.data = ptr;
    handle->end_of_storage.data = handle->begin.data + (size * 2);
    handle->end.data = ptr + size;
}

void CTL_vector_insert(CTL_vector *handle, CTL_vector_iterator *iterator, const void *data)
{
    size_t size = handle->end.T_size * CTL_vector_size(handle);
    size_t capacity = handle->end.T_size * CTL_vector_capacity(handle);
    //空间 不足 扩充一倍
    if (size >= capacity)
    {
        insert_aux(handle, iterator, 1);
        //重新配置迭代器
        memcpy(handle->begin.data + (iterator->data - handle->begin.data), data, handle->end.T_size);
    }
    else
    {
        memcpy(iterator->data + handle->end.T_size, iterator->data, handle->end.data - iterator->data);
        memcpy(iterator->data, data, handle->end.T_size);
    }
    handle->end.data += handle->end.T_size;
}

void CTL_vector_erase(CTL_vector *handle, CTL_vector_iterator *iterator)
{
    memmove(iterator->data, iterator->data + iterator->T_size, handle->end.data - iterator->data - iterator->T_size);
    handle->end.data -= handle->end.T_size;
}

void *CTL_vector_at(const CTL_vector *handle, const size_t index)
{
    return handle->begin.data + (index * handle->end.T_size);
}

void CTL_vector_iterator_move(const CTL_vector_iterator *handle, const ptrdiff_t index, CTL_vector_iterator *ret)
{
    ret->T_size = handle->T_size;
    ret->data = handle->data + (index * handle->T_size);
}

bool CTL_vector_iterator_equal(const CTL_vector_iterator *left, const CTL_vector_iterator *right)
{
    return left->data == right->data;
}

ptrdiff_t CTL_vector_iterator_diff(const CTL_vector_iterator *left, const CTL_vector_iterator *right)
{
    return (left->data - right->data) / (ptrdiff_t)left->T_size;
}

bool CTL_vector_iterator_more(const CTL_vector_iterator *left, const CTL_vector_iterator *right)
{
    return left->data > right->data;
}

size_t CTL_vector_size(const CTL_vector *handle)
{
    return CTL_vector_iterator_diff(&handle->end, &handle->begin);
}

size_t CTL_vector_capacity(const CTL_vector *handle)
{
    return CTL_vector_iterator_diff(&handle->end_of_storage, &handle->begin);
}

bool CTL_vector_empty(const CTL_vector *handle)
{
    return !handle->begin.data;
}