/**
 * @file tree_map.h
 * @author 然Y (inie0722@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "CTL/config.h"

typedef struct __rb_tree_node
{
    char *data;
    char *key;
    struct __rb_tree_node *parent;
    struct __rb_tree_node *left;
    struct __rb_tree_node *right;
    bool color;
} __rb_tree_node;

/**
 * @brief tree map key
 *
 */
typedef struct
{
    /// @brief key type size
    size_t size;

    /// @brief equal function
    bool (*equal)(const void *left, const void *right);

    /// @brief compare function
    bool (*compare)(const void *left, const void *right);
} CTL_tree_map_key;

/**
 * @brief tree map
 *
 */
typedef struct
{
    size_t T_size;
    size_t size;
    CTL_tree_map_key key;
    __rb_tree_node *header;
} CTL_tree_map;

/**
 * @brief tree map iterator
 *
 */
typedef struct
{
    size_t T_size;

    char *data;
    char *key;
    __rb_tree_node *node;
} CTL_tree_map_iterator;

/**
 * @brief create tree map
 *
 * @param handle
 * @param key
 * @param T_size element type size
 */
CTL_API extern void CTL_tree_map_new(CTL_tree_map *handle, CTL_tree_map_key *key, size_t T_size);

/**
 * @brief destroy tree map
 *
 * @param handle
 */
CTL_API extern void CTL_tree_map_delete(CTL_tree_map *handle);

/**
 * @brief clear tree map
 *
 * @param handle
 */
CTL_API extern void CTL_tree_map_clear(CTL_tree_map *handle);

/**
 * @brief get begin iterator
 *
 * @param handle
 * @param res result begin iterator
 */
CTL_API extern void CTL_tree_map_begin(const CTL_tree_map *handle, CTL_tree_map_iterator *res);

/**
 * @brief get end iterator
 *
 * @param handle
 * @param res result end iterator
 */
CTL_API extern void CTL_tree_map_end(const CTL_tree_map *handle, CTL_tree_map_iterator *res);

/**
 * @brief insert element
 *
 * @param handle
 * @param key key address
 * @param element element address
 */
CTL_API extern void CTL_tree_map_insert(CTL_tree_map *handle, const void *key, const void *element);

/**
 * @brief erases the element at a given position
 *
 * @param handle
 * @param iterator erase position
 */
CTL_API extern void CTL_tree_map_erase(CTL_tree_map *handle, CTL_tree_map_iterator *iterator);

/**
 * @brief find key
 *
 * @param handle
 * @param key key address
 * @param res result key position iterator
 */
CTL_API extern void CTL_tree_map_find(const CTL_tree_map *handle, const void *key, CTL_tree_map_iterator *res);

/**
 * @brief get the number of elements
 *
 * @param handle
 * @return size_t number of elements
 */
CTL_API extern size_t CTL_tree_map_size(const CTL_tree_map *handle);

/**
 * @brief if list is empty return true, else return false
 *
 * @param handle
 * @return true
 * @return false
 */
CTL_API extern bool CTL_tree_map_empty(const CTL_tree_map *handle);

/**
 * @brief iterator move to n
 *
 * @param handle
 * @param n positive move forward, negative move backward
 * @param res result iterator
 */
CTL_API extern void CTL_tree_map_iterator_move(const CTL_tree_map_iterator *handle, const ptrdiff_t n, CTL_tree_map_iterator *res);

/**
 * @brief if left iterator equal right iterator return true, else return false
 *
 * @param left
 * @param right
 * @return true
 * @return false
 */
CTL_API extern bool CTL_tree_map_iterator_equal(const CTL_tree_map_iterator *left, const CTL_tree_map_iterator *right);