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

typedef struct
{
    long size;
    bool (*equal)(const void *left, const void *right);
    bool (*more)(const void *left, const void *right);
} CTL_key;

typedef struct
{
    long T_size;
    size_t size;
    CTL_key key;
    __rb_tree_node *header;
} CTL_tree_map;

typedef struct
{
    long T_size;
    char *data;
    __rb_tree_node *node;
} CTL_tree_map_iterator;

extern void CTL_tree_map_new(CTL_tree_map *handle, CTL_key *key, const size_t T_size);

extern void CTL_tree_map_clear(CTL_tree_map *handle);

extern void CTL_tree_map_delete(CTL_tree_map *handle);

extern void CTL_tree_map_begin(const CTL_tree_map *handle, CTL_tree_map_iterator *ret);

extern void CTL_tree_map_end(const CTL_tree_map *handle, CTL_tree_map_iterator *ret);

extern void CTL_tree_map_insert(CTL_tree_map *handle, void *key, void *data);

extern void CTL_tree_map_erase(CTL_tree_map *handle, CTL_tree_map_iterator *iterator);

extern void CTL_tree_map_find(const CTL_tree_map *handle, const void *key, CTL_tree_map_iterator *ret);

extern void CTL_tree_map_iterator_move(const CTL_tree_map_iterator *handle, const ptrdiff_t index, CTL_tree_map_iterator *ret);

extern bool CTL_tree_map_iterator_equal(const CTL_tree_map_iterator *left, const CTL_tree_map_iterator *right);

extern size_t CTL_tree_map_size(const CTL_tree_map *handle);

extern bool CTL_tree_map_empty(const CTL_tree_map *handle);