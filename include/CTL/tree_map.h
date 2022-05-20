#include <stdbool.h>
#include <stddef.h>

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

    /// @brief less function
    bool (*less)(const void *left, const void *right);
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
    __rb_tree_node *node;
} CTL_tree_map_iterator;

/**
 * @brief create tree map
 *
 * @param handle
 * @param key
 * @param T_size element type size
 */
extern void CTL_tree_map_new(CTL_tree_map *handle, CTL_tree_map_key *key, const size_t T_size);

/**
 * @brief destroy tree map
 *
 * @param handle
 */
extern void CTL_tree_map_delete(CTL_tree_map *handle);

/**
 * @brief clear tree map
 *
 * @param handle
 */
extern void CTL_tree_map_clear(CTL_tree_map *handle);

/**
 * @brief get begin iterator
 *
 * @param handle
 * @param res result begin iterator
 */
extern void CTL_tree_map_begin(const CTL_tree_map *handle, CTL_tree_map_iterator *res);

/**
 * @brief get end iterator
 *
 * @param handle
 * @param res result end iterator
 */
extern void CTL_tree_map_end(const CTL_tree_map *handle, CTL_tree_map_iterator *res);

/**
 * @brief insert element
 *
 * @param handle
 * @param key key address
 * @param element element address
 */
extern void CTL_tree_map_insert(CTL_tree_map *handle, void *key, void *element);

/**
 * @brief erases the element at a given position
 *
 * @param handle
 * @param iterator erase position
 */
extern void CTL_tree_map_erase(CTL_tree_map *handle, CTL_tree_map_iterator *iterator);

/**
 * @brief find key
 *
 * @param handle
 * @param key key address
 * @param res result key position iterator
 */
extern void CTL_tree_map_find(const CTL_tree_map *handle, const void *key, CTL_tree_map_iterator *res);

/**
 * @brief get the number of elements
 *
 * @param handle
 * @return size_t number of elements
 */
extern size_t CTL_tree_map_size(const CTL_tree_map *handle);

/**
 * @brief if list is empty return true, else return false
 *
 * @param handle
 * @return true
 * @return false
 */
extern bool CTL_tree_map_empty(const CTL_tree_map *handle);

/**
 * @brief iterator move to n
 *
 * @param handle
 * @param n positive move forward, negative move backward
 * @param res result iterator
 */
extern void CTL_tree_map_iterator_move(const CTL_tree_map_iterator *handle, const ptrdiff_t n, CTL_tree_map_iterator *res);

/**
 * @brief if left iterator equal right iterator return true, else return false
 *
 * @param left
 * @param right
 * @return true
 * @return false
 */
extern bool CTL_tree_map_iterator_equal(const CTL_tree_map_iterator *left, const CTL_tree_map_iterator *right);