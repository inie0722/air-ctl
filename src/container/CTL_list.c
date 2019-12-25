#include <malloc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "CTL_list.h"
#include "CTL_allocator.h"

#define CTL_swap(Type, a, b) \
	do                       \
	{                        \
		Type _counter = (a); \
		(a) = (b);           \
		(b) = _counter;      \
                             \
	} while (false);

void CTL_list_new(CTL_list *handle, const size_t T_size)
{
	//构造 一个 头结点
	__CTL_list_node *node = (__CTL_list_node *)CTL_allocate(sizeof(__CTL_list_node));

	//设置其他成员状态
	handle->list = node;
	handle->list->next = node;
	handle->list->prior = node;
	handle->T_size = T_size;
}

void CTL_list_clear(CTL_list *handle)
{
	//遍历删除节点
	__CTL_list_node *node;
	for (node = handle->list->next; node != handle->list;)
	{
		__CTL_list_node *next_node = node->next;
		CTL_deallocate(node->data, handle->T_size);
		CTL_deallocate(node, sizeof(__CTL_list_node));
		node = next_node;
	}
	//恢复成员 原始状态
	handle->list = node;
	handle->list->next = node;
	handle->list->prior = node;
}

void CTL_list_delete(CTL_list *handle)
{
	CTL_list_clear(handle);
	CTL_deallocate(handle->list, sizeof(__CTL_list_node));
}

void *CTL_list_front(const CTL_list *handle)
{
	return handle->list->next->data;
}

void *CTL_list_back(const CTL_list *handle)
{
	return handle->list->prior->data;
}

CTL_list_iterator CTL_list_begin(const CTL_list *handle)
{
	CTL_list_iterator result;
	result.node = handle->list->next;
	result.data = result.node->data;
	result.T_size = handle->T_size;

	return result;
}

CTL_list_iterator CTL_list_end(const CTL_list *handle)
{
	CTL_list_iterator result;
	result.node = handle->list;
	result.data = result.node->data;
	result.T_size = handle->T_size;

	return result;
}

void CTL_list_push_back(CTL_list *handle, const void *data)
{
	CTL_list_iterator end = CTL_list_end(handle);
	CTL_list_insert(handle, &end, data);
}

void CTL_list_push_front(CTL_list *handle, const void *data)
{
	CTL_list_iterator begin = CTL_list_begin(handle);
	CTL_list_insert(handle, &begin, data);
}

void CTL_list_pop_back(CTL_list *handle)
{
	CTL_list_iterator end = CTL_list_end(handle);
	end = CTL_list_iterator_move(&end, -1);
	CTL_list_erase(handle, &end);
}

void CTL_list_pop_front(CTL_list *handle)
{
	CTL_list_iterator begin = CTL_list_begin(handle);
	CTL_list_erase(handle, &begin);
}

void CTL_list_insert(CTL_list *handle, const CTL_list_iterator *iterator, const void *data)
{
	__CTL_list_node *new_node = (__CTL_list_node *)CTL_allocate(sizeof(__CTL_list_node));

	new_node->data = (char *)CTL_allocate(handle->T_size);
	memcpy(new_node->data, data, handle->T_size);
	new_node->next = iterator->node;
	new_node->prior = iterator->node->prior;

	new_node->prior->next = new_node;
	iterator->node->prior = new_node;
}

void CTL_list_erase(CTL_list *handle, const CTL_list_iterator *iterator)
{
	iterator->node->prior->next = iterator->node->next;
	iterator->node->next->prior = iterator->node->prior;

	CTL_deallocate(iterator->data, handle->T_size);
	CTL_deallocate(iterator->node, sizeof(__CTL_list_node));
}

static void __transfer(const CTL_list_iterator *position, const CTL_list_iterator *first, const CTL_list_iterator *last)
{
	if (CTL_list_iterator_equal(position, last))
	{
		return;
	}
	last->node->prior->next = position->node;
	first->node->prior->next = last->node;
	position->node->prior->next = first->node;
	__CTL_list_node *tmp = position->node->prior;
	position->node->prior = last->node->prior;
	last->node->prior = first->node->prior;
	first->node->prior = tmp;
}

void CTL_list_splice(CTL_list *handle, const CTL_list_iterator *position, const CTL_list_iterator *first, const CTL_list_iterator *last)
{
	__transfer(position, first, last);
}

void CTL_list_merge(CTL_list *handle, CTL_list *x, bool (*compare_handler)(void *a, void *b))
{
	CTL_list_iterator first_1 = CTL_list_begin(handle);
	CTL_list_iterator last_1 = CTL_list_end(handle);

	CTL_list_iterator first_2 = CTL_list_begin(x);
	CTL_list_iterator last_2 = CTL_list_end(x);

	//循环进行合并
	while (!CTL_list_iterator_equal(&first_1, &last_1) && !CTL_list_iterator_equal(&first_2, &last_2))
	{
		if (compare_handler(first_1.data, first_2.data))
		{
			CTL_list_iterator next = CTL_list_iterator_move(&first_2, 1);
			__transfer(&first_1, &first_2, &next);
			first_2 = next;
		}
		else
		{
			first_1 = CTL_list_iterator_move(&first_1, 1);
		}
	}

	//如果x链表内还有内容, 将其全部插入handle尾部
	if (!CTL_list_iterator_equal(&first_2, &last_2))
	{
		__transfer(&last_1, &first_2, &last_2);
	}
}

void CTL_list_sort(CTL_list *handle, bool (*compare_handler)(void *a, void *b))
{
	//节点少于1 便直接退出
	if (handle->list->next == handle->list || handle->list->next->next == handle->list)
		return;

	//临时中转站
	CTL_list carry;
	CTL_list_new(&carry, handle->T_size);

	//完成排序的链表 片段
	CTL_list counter[64]; //counter[i] 可以存放 (i+1)^2的元素
	for (size_t i = 0; i < 64; i++)
	{
		CTL_list_new(&counter[i], handle->T_size);
	}

	int fill = 0; //使用counter数组的数量

	//非递归 归并排序
	while (!CTL_list_empty(handle))
	{
		CTL_list_iterator begin = CTL_list_begin(handle);
		CTL_list_iterator last = CTL_list_iterator_move(&begin, 1);
		CTL_list_iterator carry_begin = CTL_list_begin(&carry);
		CTL_list_splice(&carry, &carry_begin, &begin, &last);

		int i = 0;
		//while (i < fill && counter[i].size) 书上和标准库里是这么写的 好像i<fill没有什么意义
		while (!CTL_list_empty(&counter[i]))
		{
			CTL_list_merge(&counter[i], &carry, compare_handler);
			CTL_swap(CTL_list, carry, counter[i]);
			++i;
		}

		CTL_swap(CTL_list, carry, counter[i]);
		if (i == fill)
			++fill;
	}

	//将处理好的链表 再次合并
	for (int i = 1; i < fill; ++i)
	{
		CTL_list_merge(&counter[i], &counter[i - 1], compare_handler);
	}

	CTL_swap(CTL_list, *handle, counter[fill - 1]);

	CTL_list_delete(&carry);
	for (size_t i = 0; i < 64; i++)
	{
		CTL_list_delete(&counter[i]);
	}
}

CTL_list_iterator CTL_list_iterator_move(const CTL_list_iterator *handle, const ptrdiff_t pos)
{
	CTL_list_iterator result;
	result.T_size = handle->T_size;
	if (pos < 0)
	{
		__CTL_list_node *node = handle->node;
		for (ptrdiff_t i = pos; node && i < 0; ++i, node = node->prior)
			;
		result.node = node;
		result.data = node->data;
	}
	else
	{
		__CTL_list_node *node = handle->node;
		for (ptrdiff_t i = 0; i < pos; ++i, node = node->next)
			;
		result.node = node;
		result.data = node->data;
	}

	return result;
}

bool CTL_list_iterator_equal(const CTL_list_iterator *left, const CTL_list_iterator *right)
{
	return left->node == right->node;
}

bool CTL_list_empty(const CTL_list *handle)
{
	return handle->list->next == handle->list;
}

size_t CTL_list_size(const CTL_list *handle)
{
	size_t size = 0;
	__CTL_list_node *node;
	for (node = handle->list->next; node != handle->list;)
	{
		++size;
		__CTL_list_node *next_node = node->next;
		node = next_node;
	}

	return size;
}