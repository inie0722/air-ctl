#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "CTL_hash_map.h"
#include "CTL_allocator.h"

#define BUCKET_INITIAL_CAPACITY 16
#define LOAD_FACTOR 0.75

static unsigned long hash_code(const unsigned long hash_offset_basis, const void *key, const size_t size)
{
	const unsigned char fnv_prime = 251;
	unsigned long hash = hash_offset_basis;

	for (size_t i = 0; i < size; ++i, key = (char *)key + 1)
	{
		hash *= fnv_prime;
		hash ^= *(char *)key;
	}

	hash ^= hash >> (sizeof(unsigned long) / 2);

	return hash;
}

inline static size_t __index(unsigned long hash, size_t len)
{
	return hash & (len - 1);
}

static void resize(CTL_hash_map *handle)
{
	size_t new_B_size = handle->B_size * 2;
	__CTL_hash_map_node **new_bucket = (__CTL_hash_map_node **)CTL_allocate(new_B_size * sizeof(__CTL_hash_map_node *));
	memset(new_bucket, 0, new_B_size * sizeof(__CTL_hash_map_node *));

	for (size_t i = 0; i < handle->B_size; ++i)
	{
		for (__CTL_hash_map_node *node = handle->bucket[i]; node != NULL;)
		{
			__CTL_hash_map_node *next_node = node->next;
			unsigned long hash = hash_code(handle->hash_offset_basis, node->key, handle->K_size);
			size_t index = __index(hash, new_B_size);
			node->next = new_bucket[index];
			new_bucket[index] = node;
			node = next_node;
		}
	}

	CTL_deallocate(handle->bucket, handle->B_size * sizeof(__CTL_hash_map_node *));
	handle->bucket = new_bucket;
	handle->B_size = new_B_size;
}

void CTL_hash_map_new(CTL_hash_map *handle, size_t key_size, size_t T_size)
{
	handle->K_size = key_size;
	handle->T_size = T_size;
	handle->B_size = BUCKET_INITIAL_CAPACITY;
	handle->size = 0;
	handle->hash_offset_basis = rand();

	handle->bucket = (__CTL_hash_map_node **)CTL_allocate(sizeof(__CTL_hash_map_node *) * handle->B_size);
	memset(handle->bucket, 0, sizeof(__CTL_hash_map_node *) * handle->B_size);
}

void CTL_hash_map_clear(CTL_hash_map *handle)
{
	for (size_t i = 0; i < handle->B_size; ++i)
	{
		for (__CTL_hash_map_node *node = handle->bucket[i]; node != NULL;)
		{
			__CTL_hash_map_node *next_node = node->next;
			CTL_deallocate(node->data, handle->T_size);
			CTL_deallocate(node->key, handle->K_size);
			CTL_deallocate(node, sizeof(__CTL_hash_map_node));
			node = next_node;
		}
	}
}

void CTL_hash_map_delete(CTL_hash_map *handle)
{
	CTL_hash_map_clear(handle);
	CTL_deallocate(handle->bucket, handle->B_size * sizeof(__CTL_hash_map_node *));
}

int CTL_hash_map_insert(CTL_hash_map *handle, void *key, void *data)
{
	unsigned long hash = hash_code(handle->hash_offset_basis, key, handle->K_size);
	size_t index = __index(hash, handle->B_size);

	for (__CTL_hash_map_node *node = handle->bucket[index]; node != NULL; node = node->next)
	{
		if(!memcmp(node->key, key, handle->K_size))
		{
			return -1;
		}
	}

	++handle->size;
	if (handle->size > (handle->B_size - 1) * LOAD_FACTOR)
	{
		resize(handle);
	}

	__CTL_hash_map_node *new_node = (__CTL_hash_map_node *)CTL_allocate(sizeof(__CTL_hash_map_node));
	new_node->key = CTL_allocate(handle->K_size);
	new_node->data = CTL_allocate(handle->T_size);
	memcpy(new_node->key, key, handle->K_size);
	memcpy(new_node->data, data, handle->T_size);

	new_node->next = handle->bucket[index];
	handle->bucket[index] = new_node;
	
	return 0;
}

void CTL_hash_map_erase(CTL_hash_map *handle, CTL_hash_map_iterator *iterator)
{
	__CTL_hash_map_node *prior = *iterator->bucket;
	if (prior == iterator->node)
	{
		*iterator->bucket = iterator->node->next;
	}
	else
	{
		for (; prior->next != iterator->node; prior = prior->next)
			;
		prior = iterator->node->next;
	}

	CTL_deallocate(iterator->node->data, handle->T_size);
	CTL_deallocate(iterator->node->key, handle->K_size);
	CTL_deallocate(iterator->node, sizeof(__CTL_hash_map_node));

	--handle->size;
}

int CTL_hash_map_find(const CTL_hash_map *handle, const void *key, CTL_hash_map_iterator *ret)
{
	unsigned long hash = hash_code(handle->hash_offset_basis, key, handle->K_size);
	size_t index = __index(hash, handle->B_size);

	for (__CTL_hash_map_node *node = handle->bucket[index]; node != NULL; node = node->next)
	{
		if (!memcmp(node->key, key, handle->K_size))
		{
			ret->T_size = handle->T_size;
			ret->bucket = &handle->bucket[index];
			ret->node = node;
			ret->data = node->data;
			ret->bucket_end = handle->bucket + handle->B_size;
			return 0;
		}
	}

	return -1;
}

void CTL_hash_map_begin(const CTL_hash_map *handle, CTL_hash_map_iterator *ret)
{
	for (size_t i = 0; i < handle->B_size; i++)
	{
		if (handle->bucket[i] != NULL)
		{
			ret->T_size = handle->T_size;
			ret->bucket = &handle->bucket[i];
			ret->node = handle->bucket[i];
			ret->data = handle->bucket[i]->data;
			ret->bucket_end = handle->bucket + handle->B_size;
			return;
		}
	}

	CTL_hash_map_end(handle, ret);
}

void CTL_hash_map_end(const CTL_hash_map *handle, CTL_hash_map_iterator *ret)
{
	ret->T_size = handle->T_size;
	ret->bucket = handle->bucket + handle->B_size;
	ret->node = NULL;
	ret->data = NULL;
}

void CTL_hash_map_iterator_add(const CTL_hash_map_iterator *handle, CTL_hash_map_iterator *ret)
{
	ret->T_size = handle->T_size;
	ret->bucket_end = handle->bucket_end;
	ret->bucket = handle->bucket;

	if (handle->node->next == NULL)
	{
		++ret->bucket;
		for (; ret->bucket != ret->bucket_end && *ret->bucket == NULL; ++ret->bucket)
			;

		if (ret->bucket == ret->bucket_end)
		{
			ret->node = NULL;
			ret->data = NULL;
		}
		else
		{
			ret->node = *ret->bucket;
			ret->data = (*ret->bucket)->data;
		}
	}
	else
	{
		ret->node = handle->node->next;
		ret->data = ret->node->data;
	}
}

bool CTL_hash_map_iterator_equal(const CTL_hash_map_iterator *left, const CTL_hash_map_iterator *right)
{
	if (left->bucket != right->bucket)
	{
		return false;
	}
	else
	{
		return left->node == right->node;
	}
}

size_t CTL_hash_map_size(const CTL_hash_map *handle)
{
	return handle->size;
}

bool CTL_hash_map_empty(const CTL_hash_map *handle)
{
	return !handle->size;
}