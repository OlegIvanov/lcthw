#undef NDEBUG
#include <stdint.h>
#include <lcthw/hashmap.h>
#include <lcthw/dbg.h>
#include <lcthw/bstrlib.h>
#include <lcthw/darray_algos.h>

static int default_compare(void **a, void **b) 
{
	return bstrcmp((bstring)((HashmapNode *)*a)->key, (bstring)((HashmapNode *)*b)->key);
}

/**
 * Simple Bob Jenkins's hash algorithm taken from the
 * wikipedia description
 */
static uint32_t default_hash(void *a)
{
	size_t len = blength((bstring)a);
	char *key = bdata((bstring)a);
	uint32_t hash = 0;
	uint32_t i = 0;

	for(hash = i = 0; i < len; ++i) {
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash)
{
	Hashmap *map = calloc(1, sizeof(Hashmap));
	check_mem(map);

	map->compare = compare == NULL ? default_compare : compare;
	map->hash = hash == NULL ? default_hash : hash;

	map->buckets_number = DEFAULT_NUMBER_OF_BUCKETS;

	map->buckets = DArray_create(sizeof(DArray *), map->buckets_number);
	map->buckets->expand_rate = DEFAULT_NUMBER_OF_BUCKETS;
	map->buckets->end = map->buckets->max; // fake out expanding it

	map->counter = 0;

	check_mem(map->buckets);

	return map;

error:
	if(map) {
		Hashmap_destroy(map);
	}

	return NULL;
}

void Hashmap_destroy(Hashmap *map)
{
	int i = 0;
	int j = 0;

	if(map) {
		if(map->buckets) {	
			for(i = 0; i < DArray_count(map->buckets); i++) {
				DArray *bucket = DArray_get(map->buckets, i);
				if(bucket) {
					for(j = 0; j < DArray_count(bucket); i++) {
						free(DArray_get(bucket, j));
					}
					DArray_destroy(bucket);
				}
			}
			DArray_destroy(map->buckets);
		}

		free(map);
	}
}

static inline HashmapNode *Hashmap_node_create(int hash, void *key, void *data)
{
	HashmapNode *node = calloc(1, sizeof(HashmapNode));
	check_mem(node);

	node->key = key;
	node->data = data;
	node->hash = hash;

	return node;

error:
	return NULL;
}

static inline DArray *Hashmap_find_bucket(Hashmap *map, void *key, int create, uint32_t *hash_out)
{
	uint32_t hash = map->hash(key);
	int bucket_n = hash % map->buckets_number;
	check(bucket_n >= 0, "Invalid bucket found: %d", bucket_n);
	*hash_out = hash; // store it for the return so the caller can use it
	
	DArray *bucket = DArray_get(map->buckets, bucket_n);
	
	if(!bucket && create) {
		// new bucket, set it up
		bucket = DArray_create(sizeof(void *), map->buckets_number);
		check_mem(bucket);
		DArray_set(map->buckets, bucket_n, bucket);
	}

	return bucket;

error:
	return NULL;
}

static inline int Hashmap_get_node(Hashmap *map, uint32_t hash, DArray *bucket, void *key)
{
	// create node only for finding purposes
	HashmapNode *temp = Hashmap_node_create(0, key, NULL);

	int i = DArray_find(bucket, temp, (DArray_compare)map->compare);

	// don't need it any more
	free(temp);

	if(i >= 0) {
		HashmapNode *node = DArray_get(bucket, i);
		if(node->hash == hash) {			
			return i;
		}
	}

	return -1;
}

static int Hashmap_move_nodes(Hashmap *map)
{
	int i = 0;
	int j = 0;

	for(i = 0; i < DArray_count(map->buckets); i++) {
		DArray *bucket = DArray_get(map->buckets, i);
		if(bucket) {
			for(j = 0; j < DArray_count(bucket); j++) {
				HashmapNode *node = DArray_get(bucket, j);
					
				if(j < DArray_end(bucket) - 1) {
					DArray_set(bucket, j, DArray_last(bucket));
					DArray_set(bucket, DArray_end(bucket) - 1, node);
				}

				DArray_pop(bucket);

				int new_bucket_n = node->hash % map->buckets_number;
				DArray *new_bucket = DArray_get(map->buckets, new_bucket_n);
					
				if(!new_bucket) {
					new_bucket = DArray_create(sizeof(void *), map->buckets_number);
					DArray_set(map->buckets, new_bucket_n, new_bucket);
				}

				DArray_push(new_bucket, node);
			}
		}
	}

	return 0;
}

static int Hashmap_remove_sort_buckets(Hashmap *map)
{
	int i = 0;

	for(i = 0; i < DArray_count(map->buckets); i++) {
		DArray *bucket = DArray_get(map->buckets, i);
		if(bucket) {
			if(DArray_count(bucket) == 0) {
				DArray_clear_destroy(bucket);
				DArray_remove(map->buckets, i);
			} else {
				DArray_heapsort(bucket, (DArray_compare)map->compare);
			}
		}
	}

	return 0;
}

static inline int Hashmap_rehash(Hashmap *map, int increase_decrease_buckets)
{
	if(map->counter < DEFAULT_MAX_LOAD) {
		return -1;
	}
	
	// increase number of buckets
	if(increase_decrease_buckets) {
		if((map->counter + 1) % DEFAULT_MAX_LOAD == 1) {
			map->buckets_number += DEFAULT_NUMBER_OF_BUCKETS;

			DArray_expand(map->buckets);
			map->buckets->end = map->buckets->max; // fake out expanding it
	
			Hashmap_move_nodes(map);		
			Hashmap_remove_sort_buckets(map);

			return 0;
		}
	// decrease number of buckets
	} else {
		if(map->counter % DEFAULT_MAX_LOAD == 0) {
			map->buckets_number -= DEFAULT_NUMBER_OF_BUCKETS;
		
			Hashmap_move_nodes(map);
			Hashmap_remove_sort_buckets(map);

			DArray_contract(map->buckets);

			return 0;
		}
	}

error:
	return -1;
}

int Hashmap_set(Hashmap *map, void *key, void *data)
{
	uint32_t hash = 0;
	DArray *bucket = Hashmap_find_bucket(map, key, 1, &hash);
	check(bucket, "Error can't create bucket.");

	int i = Hashmap_get_node(map, hash, bucket, key);
	
	if(i < 0) {
		int rc = Hashmap_rehash(map, 1);

		if(rc == 0) {
			// find new bucket
			bucket = Hashmap_find_bucket(map, key, 1, &hash);

			check(bucket, "Error can't create bucket.");
		}

		HashmapNode *node = Hashmap_node_create(hash, key, data);
		check_mem(node);

		DArray_sort_add(bucket, node, (DArray_compare)map->compare);

		map->counter++;
	} else {
		HashmapNode *node = DArray_get(bucket, i);
		node->data = data;
	}

	return 0;
error:
	return -1;
}

void *Hashmap_get(Hashmap *map, void *key)
{
	uint32_t hash = 0;
	DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);
	if(!bucket) return NULL;

	int i = Hashmap_get_node(map, hash, bucket, key);
	if(i == -1) return NULL;

	HashmapNode *node = DArray_get(bucket, i);
	check(node != NULL, "Failed to get node from bucket when it should exist.");

	return node->data;

error: // fallthrough
	return NULL;
}

int Hashmap_traverse(Hashmap *map, Hashmap_traverse_cb traverse_cb)
{
	int i = 0;
	int j = 0;
	int rc = 0;

	for(i = 0; i < DArray_count(map->buckets); i++) {
		DArray *bucket = DArray_get(map->buckets, i);
		if(bucket) {
			for(j = 0; j < DArray_count(bucket); j++) {
				HashmapNode *node = DArray_get(bucket, j);
				rc = traverse_cb(node);
				if(rc != 0) return rc;
			}
		}
	}

	return 0;
}

void *Hashmap_delete(Hashmap *map, void *key)
{
	uint32_t hash = 0;
	DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);
	if(!bucket) return NULL;

	int i = Hashmap_get_node(map, hash, bucket, key);
	if(i == -1) return NULL;

	HashmapNode *node = DArray_get(bucket, i);
	void *data = node->data;
	
	if(i < DArray_end(bucket) - 1) {
		DArray_set(bucket, i, DArray_last(bucket));
		DArray_set(bucket, DArray_end(bucket) - 1, node);
	}

	DArray_free(node);
	DArray_pop(bucket);

	DArray_heapsort(bucket, (DArray_compare)map->compare);

	map->counter--;

	Hashmap_rehash(map, 0);

	return data;
}
