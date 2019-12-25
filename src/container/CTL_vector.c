#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "CTL_vector.h"
#include "CTL_allocator.h"

void CTL_vector_new(CTL_vector *handle, const size_t size, const size_t T_size)
{
    handle->begin.T_size = T_size;
    handle->end.T_size = T_size;
    handle->end_of_storage.T_size = T_size;

    handle->begin.data = (char *)CTL_allocate(T_size * size);
    handle->end.data = handle->begin.data;
    handle->end_of_storage.data = handle->begin.data + (T_size * size);
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
    return CTL_vector_iterator_move(&handle->end, -1).data;
}

CTL_vector_iterator CTL_vector_begin(const CTL_vector *handle)
{
    return handle->begin;
}

CTL_vector_iterator CTL_vector_end(const CTL_vector *handle)
{
    return handle->end;
}

void CTL_vector_push_back(CTL_vector *handle, const void *data)
{
    size_t size = handle->end.T_size * CTL_vector_size(handle);
    size_t capacity = handle->end.T_size * CTL_vector_capacity(handle);
    //空间 不足 扩充一倍
    if (size >= capacity)
    {
        char *ptr = (char *)CTL_allocate(2 * capacity);
        memcpy(ptr, handle->begin.data, size);
        CTL_deallocate(handle->begin.data, size);
        handle->begin.data = ptr;
        handle->end_of_storage.data = handle->begin.data + (capacity * 2);
        handle->end.data = ptr + size;
    }

    memcpy(handle->end.data, data, handle->end.T_size);
    handle->end.data += handle->end.T_size;
}

void CTL_vector_pop_back(CTL_vector *handle)
{
    handle->end.data -= handle->end.T_size;
}

void CTL_vector_insert(CTL_vector *handle, const CTL_vector_iterator *iterator, const void *data)
{
    size_t size = handle->end.T_size * CTL_vector_size(handle);
    size_t capacity = handle->end.T_size * CTL_vector_capacity(handle);
    //空间 不足 扩充一倍
    if (size >= capacity)
    {
        char *ptr = (char *)CTL_allocate(2 * capacity);
        //拷贝插入点前面的数据
        memcpy(ptr, handle->begin.data, iterator->data - handle->begin.data);
        //拷贝插入点后面的数据 并空出一个位置
        memcpy(ptr + (iterator->data - handle->begin.data) + handle->end.T_size, iterator->data, handle->end.data - iterator->data);
        CTL_deallocate(handle->begin.data, capacity);
        //重新配置迭代器
        memcpy(ptr + (iterator->data - handle->begin.data), data, handle->end.T_size);
        handle->begin.data = ptr;
        handle->end_of_storage.data = handle->begin.data + (capacity * 2);
        handle->end.data = ptr + size;
    }
    else
    {
        memcpy(iterator->data + handle->end.T_size, iterator->data, handle->end.data - iterator->data);
        memcpy(iterator->data, data, handle->end.T_size);
    }
    handle->end.data += handle->end.T_size;
}

void CTL_vector_erase(CTL_vector *handle, const CTL_vector_iterator *iterator)
{
    memmove(iterator->data, iterator->data + iterator->T_size, handle->end.data - iterator->data - iterator->T_size);
    handle->end.data -= handle->end.T_size;
}

CTL_vector_iterator CTL_vector_at(const CTL_vector *handle, const size_t pos)
{
    CTL_vector_iterator result;
    result.T_size = handle->end.T_size;
    result.data = handle->begin.data + (pos * handle->end.T_size);
    return result;
}

CTL_vector_iterator CTL_vector_iterator_move(const CTL_vector_iterator *handle, const ptrdiff_t pos)
{
    CTL_vector_iterator result;
    result.T_size = handle->T_size;
    result.data = handle->data + (pos * handle->T_size);

    return result;
}

bool CTL_vector_iterator_equal(const CTL_vector_iterator *left, const CTL_vector_iterator *right)
{
    return left->data == right->data;
}

ptrdiff_t CTL_vector_iterator_diff(const CTL_vector_iterator *left, const CTL_vector_iterator *right)
{
    return (left->data - right->data) / left->T_size;
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