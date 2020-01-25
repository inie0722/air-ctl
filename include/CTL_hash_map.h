#pragma once

#include <stddef.h>

typedef struct __CTL_hash_map_node
{
	void *data;
	void *key;
	struct __CTL_hash_map_node * next;
}__CTL_hash_map_node;


typedef struct
{
	__CTL_hash_map_node **bucket;
	size_t B_size;
	unsigned long hash_offset_basis;
	size_t size;
	size_t K_size;
	size_t T_size;
}CTL_hash_map;

typedef struct
{
	void *data;
	size_t T_size;
	__CTL_hash_map_node **bucket;
	__CTL_hash_map_node **bucket_end;
	__CTL_hash_map_node *node;
}CTL_hash_map_iterator;


extern void CTL_hash_map_new(CTL_hash_map *handle, size_t key_size, size_t T_size);

extern void CTL_hash_map_clear(CTL_hash_map *handle);

extern void CTL_hash_map_delete(CTL_hash_map *handle);

extern int CTL_hash_map_insert(CTL_hash_map *handle, void* key, void *data);

extern void CTL_hash_map_erase(CTL_hash_map *handle, CTL_hash_map_iterator *iterator);

extern int CTL_hash_map_find(const CTL_hash_map *handle, const void *key, CTL_hash_map_iterator *ret);

extern void CTL_hash_map_begin(const CTL_hash_map *handle, CTL_hash_map_iterator *ret);

extern void CTL_hash_map_end(const CTL_hash_map *handle, CTL_hash_map_iterator *ret);

extern void CTL_hash_map_iterator_add(const CTL_hash_map_iterator *handle, CTL_hash_map_iterator *ret);

extern bool CTL_hash_map_iterator_equal(const CTL_hash_map_iterator *left, const CTL_hash_map_iterator *right);

extern size_t CTL_hash_map_size(const CTL_hash_map *handle);

extern bool CTL_hash_map_empty(const CTL_hash_map *handle);