/**
 * @file hash_map.h
 * @author 然Y (inie0722@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <stddef.h>

#include "CTL/config.h"

typedef struct __CTL_hash_map_node
{
	void *data;
	void *key;
	struct __CTL_hash_map_node *next;
} __CTL_hash_map_node;

/**
 * @brief hash map
 *
 */
typedef struct
{
	__CTL_hash_map_node **bucket;
	size_t B_size;
	unsigned long hash_offset_basis;
	size_t size;
	size_t K_size;
	size_t T_size;
} CTL_hash_map;

/**
 * @brief hash map iterator
 *
 */
typedef struct
{
	void *data;
	size_t T_size;
	__CTL_hash_map_node **bucket;
	__CTL_hash_map_node **bucket_end;
	__CTL_hash_map_node *node;
} CTL_hash_map_iterator;

/**
 * @brief create hash map
 *
 * @param handle
 * @param key_size key type size
 * @param T_size element type size
 */
CTL_API extern void CTL_hash_map_new(CTL_hash_map *handle, size_t key_size, size_t T_size);

/**
 * @brief clear hash map
 *
 * @param handle
 */
CTL_API extern void CTL_hash_map_clear(CTL_hash_map *handle);

/**
 * @brief destroy hash map
 *
 * @param handle
 */
CTL_API extern void CTL_hash_map_delete(CTL_hash_map *handle);

/**
 * @brief insert element
 *
 * @param handle
 * @param key key address
 * @param element element address
 * @return int 0 if success, -1 if failed
 */
CTL_API extern int CTL_hash_map_insert(CTL_hash_map *handle, const void *key, const void *element);

/**
 * @brief erases the element at a given position
 *
 * @param handle
 * @param iterator erase position
 */
CTL_API extern void CTL_hash_map_erase(CTL_hash_map *handle, CTL_hash_map_iterator *iterator);

/**
 * @brief find key
 *
 * @param handle
 * @param key key address
 * @param res result key position iterator
 * @return int 0 if success, -1 if failed
 */
CTL_API extern int CTL_hash_map_find(const CTL_hash_map *handle, const void *key, CTL_hash_map_iterator *res);

/**
 * @brief get begin iterator
 *
 * @param handle
 * @param res result begin iterator
 */
CTL_API extern void CTL_hash_map_begin(const CTL_hash_map *handle, CTL_hash_map_iterator *res);

/**
 * @brief get end iterator
 *
 * @param handle
 * @param res result end iterator
 */
CTL_API extern void CTL_hash_map_end(const CTL_hash_map *handle, CTL_hash_map_iterator *res);

/**
 * @brief get the number of elements
 *
 * @param handle
 * @return size_t number of elements
 */
CTL_API extern size_t CTL_hash_map_size(const CTL_hash_map *handle);

/**
 * @brief if hash map is empty return true, else return false
 *
 * @param handle
 * @return true
 * @return false
 */
CTL_API extern bool CTL_hash_map_empty(const CTL_hash_map *handle);

/**
 * @brief get iterator next
 *
 * @param handle
 * @param res result iterator
 */
CTL_API extern void CTL_hash_map_iterator_next(const CTL_hash_map_iterator *handle, CTL_hash_map_iterator *res);

/**
 * @brief if left iterator equal right iterator return true, else return false
 *
 * @param left
 * @param right
 * @return true
 * @return false
 */
CTL_API extern bool CTL_hash_map_iterator_equal(const CTL_hash_map_iterator *left, const CTL_hash_map_iterator *right);