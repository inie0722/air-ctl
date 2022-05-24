/**
 * @file list.h
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

typedef struct __CTL_list_node
{
	char *data;
	struct __CTL_list_node *prior;
	struct __CTL_list_node *next;
} __CTL_list_node;

/**
 * @brief list iterator
 *
 */
typedef struct
{
	char *data;
	size_t T_size;
	__CTL_list_node *node;
} CTL_list_iterator;

/**
 * @brief list
 *
 */
typedef struct
{
	size_t T_size;
	__CTL_list_node *list;
} CTL_list;

/**
 * @brief create a list
 *
 * @param handle
 * @param T_size element type size
 */
extern void CTL_list_new(CTL_list *handle, const size_t T_size);

/**
 * @brief destroy list
 *
 * @param handle
 */
extern void CTL_list_delete(CTL_list *handle);

/**
 * @brief clear list
 *
 * @param handle
 */
extern void CTL_list_clear(CTL_list *handle);

/**
 * @brief get first element
 *
 * @param handle
 * @return void* first element address
 */
extern void *CTL_list_front(const CTL_list *handle);

/**
 * @brief get last element
 *
 * @param handle
 * @return void* last element address
 */
extern void *CTL_list_back(const CTL_list *handle);

/**
 * @brief get begin iterator
 *
 * @param handle
 * @param res result begin iterator
 */
extern void CTL_list_begin(const CTL_list *handle, CTL_list_iterator *res);

/**
 * @brief get end iterator
 *
 * @param handle
 * @param res result end iterator
 */
extern void CTL_list_end(const CTL_list *handle, CTL_list_iterator *res);

/**
 * @brief adds an element to the end
 *
 * @param handle
 * @param data element address
 */
extern void CTL_list_push_back(CTL_list *handle, const void *element);

/**
 * @brief adds an element to the front
 *
 * @param handle
 * @param element element address
 */
extern void CTL_list_push_front(CTL_list *handle, const void *element);

/**
 * @brief erases the last element
 *
 * @param handle
 */
extern void CTL_list_pop_back(CTL_list *handle);

/**
 * @brief erases the first element
 *
 * @param handle
 */
extern void CTL_list_pop_front(CTL_list *handle);

/**
 * @brief inserts an element at a given position
 *
 * @param handle
 * @param iterator insert position
 * @param element element address
 */
extern void CTL_list_insert(CTL_list *handle, CTL_list_iterator *iterator, const void *element);

/**
 * @brief erases the element at a given position
 *
 * @param handle
 * @param iterator erase position
 */
extern void CTL_list_erase(CTL_list *handle, CTL_list_iterator *iterator);

/**
 * @brief get the number of elements
 *
 * @param handle
 * @return size_t number of elements
 */
extern size_t CTL_list_size(const CTL_list *handle);

/**
 * @brief if list is empty return true, else return false
 *
 * @param handle
 * @return true
 * @return false
 */
extern bool CTL_list_empty(const CTL_list *handle);

/**
 * @brief splices the range [first, last) into the range starting at position
 *
 * @param handle
 * @param position
 * @param first
 * @param last
 */
extern void CTL_list_splice(CTL_list *handle, const CTL_list_iterator *position, const CTL_list_iterator *first, const CTL_list_iterator *last);

/**
 * @brief merges two sorted lists
 *
 * @param handle
 * @param other another container to merge
 * @param compare_handler compare function
 */
extern void CTL_list_merge(CTL_list *handle, CTL_list *other, bool (*compare_handler)(void *a, void *b));

/**
 * @brief sorts the elements
 *
 * @param handle
 * @param compare_handler compare function
 */
extern void CTL_list_sort(CTL_list *handle, bool (*compare_handler)(void *a, void *b));

/**
 * @brief iterator move to n
 *
 * @param handle
 * @param n positive move forward, negative move backward
 * @param res result iterator
 */
extern void CTL_list_iterator_move(const CTL_list_iterator *handle, const ptrdiff_t index, CTL_list_iterator *res);

/**
 * @brief if left iterator equal right iterator return true, else return false
 *
 * @param left
 * @param right
 * @return true
 * @return false
 */
extern bool CTL_list_iterator_equal(const CTL_list_iterator *left, const CTL_list_iterator *right);
