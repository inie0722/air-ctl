#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "CTL/hash_map.h"
#include "CTL/allocator.h"

#define BUCKET_INITIAL_CAPACITY 16
#define LOAD_FACTOR 0.75

static unsigned long hash_code(const unsigned long hash_offset_basis, const void *key, size_t size)
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

int CTL_hash_map_insert(CTL_hash_map *handle, const void *key, const void *element)
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
	memcpy(new_node->data, element, handle->T_size);

	new_node->next = handle->bucket[index];
	handle->bucket[index] = new_node;
	
	return 0;
}

void CTL_hash_map_erase(CTL_hash_map *handle, CTL_hash_map_iterator *iterator)
{
	if (*iterator->bucket == iterator->node)
	{
		*iterator->bucket = iterator->node->next;
	}
	else
	{
		__CTL_hash_map_node *prior = *iterator->bucket;
		for (; prior->next != iterator->node; prior = prior->next)
			;
		prior->next = iterator->node->next;
	}

	CTL_deallocate(iterator->node->data, handle->T_size);
	CTL_deallocate(iterator->node->key, handle->K_size);
	CTL_deallocate(iterator->node, sizeof(__CTL_hash_map_node));

	--handle->size;
}

int CTL_hash_map_find(const CTL_hash_map *handle, const void *key, CTL_hash_map_iterator *res)
{
	unsigned long hash = hash_code(handle->hash_offset_basis, key, handle->K_size);
	size_t index = __index(hash, handle->B_size);

	for (__CTL_hash_map_node *node = handle->bucket[index]; node != NULL; node = node->next)
	{
		if (!memcmp(node->key, key, handle->K_size))
		{
			res->T_size = handle->T_size;
			res->bucket = &handle->bucket[index];
			res->node = node;
			res->data = node->data;
			res->bucket_end = handle->bucket + handle->B_size;
			return 0;
		}
	}

	return -1;
}

void CTL_hash_map_begin(const CTL_hash_map *handle, CTL_hash_map_iterator *res)
{
	for (size_t i = 0; i < handle->B_size; i++)
	{
		if (handle->bucket[i] != NULL)
		{
			res->T_size = handle->T_size;
			res->bucket = &handle->bucket[i];
			res->node = handle->bucket[i];
			res->data = handle->bucket[i]->data;
			res->bucket_end = handle->bucket + handle->B_size;
			return;
		}
	}

	CTL_hash_map_end(handle, res);
}

void CTL_hash_map_end(const CTL_hash_map *handle, CTL_hash_map_iterator *res)
{
	res->T_size = handle->T_size;
	res->bucket = handle->bucket + handle->B_size;
	res->node = NULL;
	res->data = NULL;
}

void CTL_hash_map_iterator_next(const CTL_hash_map_iterator *handle, CTL_hash_map_iterator *res)
{
	res->T_size = handle->T_size;
	res->bucket_end = handle->bucket_end;
	res->bucket = handle->bucket;

	if (handle->node->next == NULL)
	{
		++res->bucket;
		for (; res->bucket != res->bucket_end && *res->bucket == NULL; ++res->bucket)
			;

		if (res->bucket == res->bucket_end)
		{
			res->node = NULL;
			res->data = NULL;
		}
		else
		{
			res->node = *res->bucket;
			res->data = (*res->bucket)->data;
		}
	}
	else
	{
		res->node = handle->node->next;
		res->data = res->node->data;
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