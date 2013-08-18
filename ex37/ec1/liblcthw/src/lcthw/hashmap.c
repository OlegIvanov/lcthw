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

Hashmap *Hashmap_create_advanced(Hashmap_compare compare, Hashmap_hash hash, uint32_t buckets_number, uint32_t max_load)
{
	Hashmap *map = calloc(1, sizeof(Hashmap));
	check_mem(map);

	map->compare = compare == NULL ? default_compare : compare;
	map->hash = hash == NULL ? default_hash : hash;

	map->default_number_of_buckets = (buckets_number == 0) ? DEFAULT_NUMBER_OF_BUCKETS : buckets_number;
	map->default_max_load = (max_load == 0) ? DEFAULT_MAX_LOAD : max_load;

	map->buckets_number = map->default_number_of_buckets;

	map->buckets = DArray_create(sizeof(DArray *), map->default_number_of_buckets);
	map->buckets->expand_rate = map->default_number_of_buckets;
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

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash)
{
	return Hashmap_create_advanced(compare, hash, 0, 0);
}

void Hashmap_destroy(Hashmap *map)
{
	int i = 0;

	if(map) {
		if(map->buckets) {	
			for(i = 0; i < DArray_count(map->buckets); i++) {
				DArray *bucket = DArray_get(map->buckets, i);
				if(bucket) {
					DArray_clear_destroy(bucket);
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
	uint32_t hash;

	for(i = 0; i < DArray_count(map->buckets); i++) {
		DArray *bucket = DArray_get(map->buckets, i);
		if(bucket) {
			for(j = 0; j < DArray_count(bucket); j++) {
				HashmapNode *node = DArray_get(bucket, j);
					
				if(DArray_count(bucket) != 1) {
					DArray_set(bucket, j, DArray_last(bucket));
					DArray_set(bucket, DArray_end(bucket) - 1, node);
				}

				DArray_pop(bucket);

				DArray *new_bucket = Hashmap_find_bucket(map, node->key, 1, &hash);

				DArray_push(new_bucket, node);
			}
		}
	}

	for(i = 0; i < DArray_count(map->buckets); i++) {
		DArray *bucket = DArray_get(map->buckets, i);
		if(bucket) {
			if(DArray_count(bucket) == 0) {
				DArray_destroy(bucket);
				DArray_remove(map->buckets, i);
			}
		}
	}

	for(i = 0; i < DArray_count(map->buckets); i++) {
		DArray *bucket = DArray_get(map->buckets, i);
		if(bucket) {
			DArray_heapsort(bucket, (DArray_compare)map->compare);
		}
	}

	return 0;
}

static inline int Hashmap_rehash(Hashmap *map, int increase_buckets)
{
	if(map->counter == 0) return 0;

	if(map->counter % map->default_max_load == 0) {
		// increase number of buckets
		if(increase_buckets) {
			map->buckets_number += map->default_number_of_buckets;

			DArray_expand(map->buckets);
			map->buckets->end = map->buckets->max; // fake out expanding it

			Hashmap_move_nodes(map);

			return map->buckets_number;
		// decrease number of buckets if it is possible
		} else if(map->buckets_number > map->default_number_of_buckets) {
			map->buckets_number -= map->default_number_of_buckets;

			Hashmap_move_nodes(map);

			DArray_contract(map->buckets);

			return map->buckets_number;
		}
	}

	return 0;
}

int Hashmap_set(Hashmap *map, void *key, void *data)
{
	check(map, "map can't be NULL");
	check(key, "key can't be NULL");
	check(data, "data can't be NULL");

	uint32_t hash = 0;
	DArray *bucket = Hashmap_find_bucket(map, key, 1, &hash);
	check(bucket, "Error can't create bucket.");

	int i = Hashmap_get_node(map, hash, bucket, key);
	
	if(i < 0) {
		if(Hashmap_rehash(map, 1) > 0) {
			// find new bucket
			bucket = Hashmap_find_bucket(map, key, 1, &hash);

			check(bucket, "Error can't create bucket.");
		}

		HashmapNode *node = Hashmap_node_create(hash, key, data);
		check_mem(node);

		int rc = DArray_sort_add(bucket, node, (DArray_compare)map->compare);
		check(rc == 0, "DArray_sort_add failed.");

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
	
	if(DArray_count(bucket) != 1) {
		DArray_set(bucket, i, DArray_last(bucket));
		DArray_set(bucket, DArray_end(bucket) - 1, node);
	}

	DArray_free(node);
	DArray_pop(bucket);

	map->counter--;

	DArray_heapsort(bucket, (DArray_compare)map->compare);

	if(DArray_count(bucket) == 0) {
		DArray_destroy(bucket);
		DArray_remove(map->buckets, hash % map->buckets_number);
	}
	
	Hashmap_rehash(map, 0);

	return data;
}
