#include <string.h>
#include <stdbool.h>

#include "CTL/allocator.h"
#include "CTL/deque.h"

static inline void set_node(CTL_deque_iterator *dest, char **src, size_t data_size, size_t T_size)
{
    dest->node = src;
    dest->first = *src;
    dest->last = *src + (data_size * T_size);
}

static void reallocate_map(CTL_deque *handle, size_t nodes_to_add, bool front);

static void push_aux_back(CTL_deque *handle, const void *element);

static void push_aux_front(CTL_deque *handle, const void *element);

static void pop_aux_back(CTL_deque *handle);

static void pop_aux_front(CTL_deque *handle);

static void insert_aux(CTL_deque *handle, const CTL_deque_iterator *iterator, const void *element);

void CTL_deque_new(CTL_deque *handle, const size_t data_size, const size_t T_size)
{
    handle->data_size = data_size;
    //一个map至少 拥有8个节点
    handle->map_size = 8;
    handle->map = (char **)CTL_allocate(sizeof(char *) * handle->map_size);

    //先分配一个缓存区
    char **start = handle->map + ((handle->map_size - 1) / 2);
    char **finish = start;
    *start = (char *)CTL_allocate(T_size * handle->data_size);
    //设置 begin和end 两个迭代器
    set_node(&handle->begin, start, handle->data_size, T_size);
    handle->begin.data = *start;
    handle->begin.T_size = T_size;

    set_node(&handle->end, finish, handle->data_size, T_size);
    handle->end.data = *finish;
    handle->end.T_size = T_size;
}

void CTL_deque_clear(CTL_deque *handle)
{
    //删除 第二个 - 倒数第二个 缓存区

    // 删除前后两个数组的元素.
    if (handle->begin.node != handle->end.node)
    {
        //释放缓存区
        for (char **node = handle->begin.node + 1; node <= handle->end.node; ++node)
        {
            CTL_deallocate(*node, handle->end.T_size * handle->data_size);
        }
    }

    //重新调整map
    char **start = handle->begin.node;
    char **finish = start;
    //设置 begin和end 两个迭代器
    set_node(&handle->begin, start, handle->data_size, handle->end.T_size);
    handle->begin.data = *start;
    set_node(&handle->end, finish, handle->data_size, handle->end.T_size);
    handle->end.data = *finish;
}

void CTL_deque_delete(CTL_deque *handle)
{
    CTL_deque_clear(handle);
    CTL_deallocate(*handle->begin.node, handle->end.T_size * handle->data_size);
    CTL_deallocate(handle->map, sizeof(char *) * handle->map_size);
}

void *CTL_deque_front(const CTL_deque *handle)
{
    return handle->begin.data;
}

void *CTL_deque_back(const CTL_deque *handle)
{
    CTL_deque_iterator back;
    CTL_deque_iterator_move(&handle->end, -1, &back);
    return back.data;
}

void CTL_deque_begin(const CTL_deque *handle, CTL_deque_iterator *res)
{
    *res = handle->begin;
}

void CTL_deque_end(const CTL_deque *handle, CTL_deque_iterator *res)
{
    *res = handle->end;
}

static void reallocate_map(CTL_deque *handle, size_t nodes_to_add, bool front)
{
    size_t num_old_nodes = handle->end.node - handle->begin.node + 1;
    size_t num_new_nodes = num_old_nodes + nodes_to_add;

    char **new_start;

    if (handle->map_size > 2 * num_new_nodes)
    {
        //map空间足够
        new_start = handle->map + (handle->map_size - num_new_nodes) / 2 + (front ? nodes_to_add : 0);

        memmove(new_start, handle->begin.node, sizeof(char *) * (handle->end.node - handle->begin.node + 1));
    }
    else
    {
        //重新 分配 map空间
        size_t new_map_size = handle->map_size + 2 + (handle->map_size > nodes_to_add ? handle->map_size : nodes_to_add);
        char **new_map = (char **)CTL_allocate(sizeof(char *) * new_map_size);

        new_start = new_map + (new_map_size - num_new_nodes) / 2 + (front ? nodes_to_add : 0);

        memcpy(new_start, handle->begin.node, sizeof(char *) * (handle->end.node - handle->begin.node + 1));
        CTL_deallocate(handle->map, sizeof(char *) * handle->map_size);

        handle->map = new_map;
        handle->map_size = new_map_size;
    }

    set_node(&handle->begin, new_start, handle->data_size, handle->end.T_size);
    handle->begin.data = *new_start;
    handle->end.data = *(new_start + num_old_nodes - 1) + (handle->end.data - handle->end.first);
    set_node(&handle->end, new_start + num_old_nodes - 1, handle->data_size, handle->end.T_size);
}

static void push_aux_back(CTL_deque *handle, const void *element)
{
    memcpy(handle->end.data, element, handle->end.T_size);

    if (2 > handle->map_size - (handle->end.node - handle->map))
    {
        //尾端 空间不足
        reallocate_map(handle, 1, false);
    }
    //分配一个新节点
    *(handle->end.node + 1) = (char *)CTL_allocate(handle->end.T_size * handle->data_size);
    //设置end 迭代器
    ++handle->end.node;
    set_node(&handle->end, handle->end.node, handle->data_size, handle->end.T_size);
    handle->end.data = handle->end.first;
}

void CTL_deque_push_back(CTL_deque *handle, const void *element)
{
    if (handle->end.data != handle->end.last - handle->end.T_size)
    {
        //最后一个缓存区 尚有 空间
        memcpy(handle->end.data, element, handle->end.T_size);
        handle->end.data += handle->end.T_size;
    }
    else
    {

        push_aux_back(handle, element);
    }
    ++handle->size;
}

static void push_aux_front(CTL_deque *handle, const void *element)
{
    if (1 > handle->begin.node - handle->map)
    {
        //前端 空间不足
        reallocate_map(handle, 1, true);
    }
    //分配一个新节点
    *(handle->begin.node - 1) = (char *)CTL_allocate(handle->end.T_size * handle->data_size);
    //设置begin 迭代器
    --handle->begin.node;
    set_node(&handle->begin, handle->begin.node, handle->data_size, handle->end.T_size);
    handle->begin.data = handle->begin.last - handle->end.T_size;

    memcpy(handle->begin.data, element, handle->end.T_size);
}

void CTL_deque_push_front(CTL_deque *handle, const void *element)
{
    if (handle->begin.data != handle->begin.first)
    {
        //第一个缓存区 尚有 空间
        handle->begin.data -= handle->end.T_size;
        memcpy(handle->begin.data, element, handle->end.T_size);
    }
    else
    {
        push_aux_front(handle, element);
    }
    ++handle->size;
}

static void pop_aux_back(CTL_deque *handle)
{
    CTL_deallocate(*handle->end.node, handle->end.T_size * handle->data_size);
    --handle->end.node;
    set_node(&handle->end, handle->end.node, handle->data_size, handle->end.T_size);
    handle->end.data = handle->end.last - handle->end.T_size;
}

void CTL_deque_pop_back(CTL_deque *handle)
{

    if (handle->end.data != handle->end.first)
    {
        //缓存区 更多元素
        //最后一个缓存区 尚有 一个空间
        handle->end.data -= handle->end.T_size;
    }
    else
    {
        //仅有一个元素
        pop_aux_back(handle);
    }
    --handle->size;
}

static void pop_aux_front(CTL_deque *handle)
{
    CTL_deallocate(*handle->begin.node, handle->end.T_size * handle->data_size);
    ++handle->begin.node;
    set_node(&handle->begin, handle->begin.node, handle->data_size, handle->end.T_size);
    handle->begin.data = handle->begin.first;
}

void CTL_deque_pop_front(CTL_deque *handle)
{
    if (handle->begin.data != handle->begin.last - handle->end.T_size)
    {
        //缓存区 更多元素
        //最后一个缓存区 尚有 一个空间
        handle->begin.data += handle->end.T_size;
    }
    else
    {
        //仅有一个元素
        pop_aux_front(handle);
    }
    --handle->size;
}

static void insert_aux(CTL_deque *handle, const CTL_deque_iterator *iterator, const void *element)
{
    ptrdiff_t index = CTL_deque_iterator_diff(iterator, &handle->begin);

    if (index < handle->size / 2)
    {
        CTL_deque_push_front(handle, handle->begin.data);

        CTL_deque_iterator result;
        CTL_deque_iterator_move(&handle->begin, 1, &result);

        CTL_deque_iterator first;
        CTL_deque_iterator_move(&result, 1, &first);

        CTL_deque_iterator last;
        CTL_deque_iterator_move(&handle->begin, index + 1, &last);

        while (!CTL_deque_iterator_equal(&first, &last))
        {
            memcpy(result.data, first.data, iterator->T_size);
            CTL_deque_iterator_move(&first, 1, &first);
            CTL_deque_iterator_move(&result, 1, &result);
        }

        CTL_deque_iterator_move(&last, -1, &last);
        memcpy(last.data, element, iterator->T_size);
    }
    else
    {

        CTL_deque_push_back(handle, handle->end.data);

        CTL_deque_iterator result;
        CTL_deque_iterator_move(&handle->end, -1, &result);

        CTL_deque_iterator first;
        CTL_deque_iterator_move(&result, -1, &first);

        CTL_deque_iterator last;
        CTL_deque_iterator_move(&handle->begin, index - 1, &last);

        while (!CTL_deque_iterator_equal(&first, &last))
        {
            memcpy(result.data, first.data, iterator->T_size);
            CTL_deque_iterator_move(&first, -1, &first);
            CTL_deque_iterator_move(&result, -1, &result);
        }

        memcpy(iterator->data, element, iterator->T_size);
    }
}

void CTL_deque_insert(CTL_deque *handle, CTL_deque_iterator *iterator, const void *element)
{
    if (CTL_deque_iterator_equal(iterator, &handle->begin))
    {
        CTL_deque_push_front(handle, element);
    }
    else if (CTL_deque_iterator_equal(iterator, &handle->end))
    {
        CTL_deque_push_back(handle, element);
    }
    else
    {
        insert_aux(handle, iterator, element);
    }
}

void CTL_deque_erase(CTL_deque *handle, CTL_deque_iterator *iterator)
{
    ptrdiff_t index = CTL_deque_iterator_diff(iterator, &handle->begin);

    if (index < handle->size / 2)
    {
        CTL_deque_iterator result = *iterator;
        CTL_deque_iterator first;
        CTL_deque_iterator_move(iterator, -1, &first);

        CTL_deque_iterator last;
        CTL_deque_iterator_move(&handle->begin, -1, &last);

        while (!CTL_deque_iterator_equal(&first, &last))
        {
            memcpy(result.data, first.data, iterator->T_size);
            CTL_deque_iterator_move(&first, -1, &first);
            CTL_deque_iterator_move(&result, -1, &result);
        }

        CTL_deque_pop_front(handle);
    }
    else
    {
        CTL_deque_iterator result = *iterator;
        CTL_deque_iterator first;
        CTL_deque_iterator_move(iterator, 1, &first);

        CTL_deque_iterator last;
        CTL_deque_iterator_move(&handle->end, 1, &last);

        while (!CTL_deque_iterator_equal(&first, &last))
        {
            memcpy(result.data, first.data, iterator->T_size);
            CTL_deque_iterator_move(&first, 1, &first);
            CTL_deque_iterator_move(&result, 1, &result);
        }

        CTL_deque_pop_back(handle);
    }
}

void *CTL_deque_at(const CTL_deque *handle, size_t index)
{
    CTL_deque_iterator res;
    CTL_deque_iterator_move(&handle->begin, index, &res);
    return res.data;
}

void CTL_deque_iterator_move(const CTL_deque_iterator *handle, ptrdiff_t n, CTL_deque_iterator *res)
{
    *res = *handle;
    ptrdiff_t data_size = handle->last - handle->first;
    ptrdiff_t offset = (n * (ptrdiff_t)handle->T_size) + (handle->data - handle->first);

    if (offset >= 0 && offset < data_size)
    {
        res->data = handle->data + (n * (ptrdiff_t)handle->T_size);
    }
    else
    {
        ptrdiff_t node_offset = offset > 0 ? offset / data_size : ((-offset - 1) / data_size - 1);
        set_node(res, handle->node + node_offset, data_size, 1);
        res->data = res->first + offset - (node_offset * data_size);
    }
}

bool CTL_deque_iterator_equal(const CTL_deque_iterator *left, const CTL_deque_iterator *right)
{
    return left->data == right->data;
}

bool CTL_deque_iterator_less(const CTL_deque_iterator *left, const CTL_deque_iterator *right)
{
    if (left->node == right->node)
    {
        return left->data < right->data;
    }
    else
    {
        return left->node < right->node;
    }
}

ptrdiff_t CTL_deque_iterator_diff(const CTL_deque_iterator *left, const CTL_deque_iterator *right)
{
    if (left->node == right->node)
    {
        return (left->data - right->data) / (ptrdiff_t)left->T_size;
    }

    ptrdiff_t data_size = left->last - left->first;
    ptrdiff_t left_diff, right_diff;
    left_diff = (left->data - left->first);
    right_diff = (right->last - right->data);

    if (left->node < right->node)
    {
        return (left_diff + right_diff + ((left->node - right->node - 1) * data_size)) / (ptrdiff_t)left->T_size;
    }
    else
    {
        return (left_diff + right_diff + ((left->node - right->node - 1) * data_size)) / (ptrdiff_t)left->T_size;
    }
}

size_t CTL_deque_size(const CTL_deque *handle)
{
    return handle->size;
}

bool CTL_deque_empty(const CTL_deque *handle)
{
    return !handle->size;
}
