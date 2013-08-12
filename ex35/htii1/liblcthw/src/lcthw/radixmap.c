/*
* Based on code by Andre Reinald then heavily modified by Zed A. Shaw.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lcthw/radixmap.h>
#include <lcthw/dbg.h>

RadixMap *RadixMap_create(size_t max)
{
	RadixMap *map = calloc(sizeof(RadixMap), 1);
	check_mem(map);

	map->contents = calloc(sizeof(RMElement), max + 1);
	check_mem(map->contents);

	map->temp = calloc(sizeof(RMElement), max + 1);
	check_mem(map->temp);

	map->max = max;
	map->end = 0;
	map->biggest_key = 0;

	return map;
error:
	return NULL;
}

void RadixMap_destroy(RadixMap *map)
{
	if(map) {
		free(map->contents);
		free(map->temp);
		free(map);
	}
}

#define ByteOf(x, y) (((uint8_t *)x)[(y)])

static inline void radix_sort(short offset, uint64_t max, uint64_t *source, uint64_t *dest)
{
	uint64_t count[256] = {0};
	uint64_t *cp = NULL;
	uint64_t *sp = NULL;
	uint64_t *end = NULL;
	uint64_t s = 0;
	uint64_t c = 0;

	// count occurences of every byte value
	for(sp = source, end = source + max; sp < end; sp++) {
		count[ByteOf(sp, offset)]++;
	}

	// transform count into index by summing elements and storing into same array
	for(s = 0, cp = count, end = count + 256; cp < end; cp++) {
		c = *cp;
		*cp = s;
		s += c;
	}

	// fill dest with the right values in the right place
	for(sp = source, end = source + max; sp < end; sp++) {
		cp = count + ByteOf(sp, offset);
		dest[*cp] = *sp;
		++(*cp);
	}
}

static void RadixMap_sort_optimized(RadixMap *map, uint64_t max, size_t starting_index, uint32_t biggest_key)
{
	uint64_t *source = &map->contents[starting_index].raw;
	uint64_t *temp = &map->temp[starting_index].raw;
	
	int i = 0;
	int significant_digits = 0;

	for(i = 0; i < 4; i++) {
		if(ByteOf(&biggest_key, i) != 0) significant_digits++;
	}

	switch(significant_digits) {
		case 1:
			radix_sort(0, max, source, temp);
			memcpy(source, temp, max * sizeof(uint64_t));
			break;

		case 2:
			radix_sort(0, max, source, temp);
			radix_sort(1, max, temp, source);
			break;

		case 3:
			radix_sort(0, max, source, temp);
			radix_sort(1, max, temp, source);
			radix_sort(2, max, source, temp);
			memcpy(source, temp, max * sizeof(uint64_t));
			break;

		case 4:
			radix_sort(0, max, source, temp);
			radix_sort(1, max, temp, source);
			radix_sort(2, max, source, temp);
			radix_sort(3, max, temp, source);
			break;
	}
}

void RadixMap_sort(RadixMap *map)
{
	RadixMap_sort_optimized(map, map->end, 0, UINT32_MAX);
}

RMElement *RadixMap_find(RadixMap *map, uint32_t to_find)
{
	int low = 0;
	int high = map->end - 1;
	RMElement *data = map->contents;

	while(low <= high) {
		int middle = low + (high - low) / 2;
		uint32_t key = data[middle].data.key;

		if(to_find < key) {
			high = middle - 1;
		} else if(to_find > key) {
			low = middle + 1;
		} else {
			return &data[middle];
		}
	}

	return NULL;
}

static int RadixMap_find_min_position(RadixMap *map, uint32_t to_find)
{
	int low = 0;
	int high = map->end - 1;
	RMElement *data = map->contents;
	int middle = low;

	while(low <= high) {
		middle = low + (high - low) / 2;
		uint32_t key = data[middle].data.key;

		if(to_find < key) {
			high = middle - 1;
		} else if(to_find > key) {
			low = middle + 1;
		} else {
			return middle;
		}
	}

	return middle;
}

int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value, int optimize)
{
	check(key < UINT32_MAX, "Key can't be equal to UINT32_MAX.");

	RMElement element = {.data = {.key = key, .value = value}};
	check(map->end + 1 < map->max, "RadixMap is full.");

	if(optimize) {
		int min_position = RadixMap_find_min_position(map, key);

		map->contents[map->end++] = element;

		if(key > map->biggest_key) map->biggest_key = key;

		// sort starting from 'min_position' index 'map->end - min_position' elements
		RadixMap_sort_optimized(map, map->end - min_position, min_position, map->biggest_key);

	} else {
		map->contents[map->end++] = element;

		if(key > map->biggest_key) map->biggest_key = key;

		RadixMap_sort(map);
	}

	return 0;
error:
	return -1;
}

int RadixMap_delete(RadixMap *map, RMElement *el)
{
	check(map->end > 0, "There is nothing to delete.");
	check(el != NULL, "Can't delete a NULL element.");

	el->data.key = UINT32_MAX;

	if(map->end > 1) {
		// don't bother resorting a map of 1 length
		RadixMap_sort(map);
	}

	map->end--;

	return 0;
error:
	return -1;
}
