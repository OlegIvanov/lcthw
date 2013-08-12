#include "minunit.h"
#include <lcthw/radixmap.h>
#include <time.h>

#define BILLION 1000000000UL

#define MAPS_COUNT 1000L
#define MAP_SIZE 1000L

static unsigned long get_diff(struct timespec start, struct timespec end)
{
	return (unsigned long)(end.tv_sec - start.tv_sec) * BILLION + (unsigned long)(end.tv_nsec - start.tv_nsec);
}

static int check_order(RadixMap *map)
{
	RMElement d1, d2;
	unsigned int i = 0;

	// only signal errors if any (should not be)
	for(i = 0; map->end > 0 && i < map->end - 1; i++) {
		d1 = map->contents[i];
		d2 = map->contents[i + 1];
		
		if(d1.data.key > d2.data.key) {
			debug("FAIL:i=%u, key: %u, value: %u, equals max? %d\n", i, d1.data.key, d1.data.value,
					d2.data.key == UINT32_MAX);
			return 0;
		}
	}

	return 1;
}

static int make_random(RadixMap *map)
{
	size_t i = 0;

	for(i = 0; i < map->max - 1; i++) {
		uint32_t key = (uint32_t)(rand() | rand() << 16);
		check(RadixMap_add_optimized(map, key, i) == 0, "Failed to add key %u.", key);
		mu_assert(check_order(map), "RadixMap isn't sorted after add.");
	}

	return i;
error:
	return 0;
}

static int test_search(RadixMap *map)
{
	unsigned i = 0;

	RMElement *d = NULL;
	RMElement *found = NULL;

	for(i = map->end / 2; i < map->end; i++) {
		d = &map->contents[i];
		found = RadixMap_find(map, d->data.key);
		check(found != NULL, "Didn't find %u at %u.", d->data.key, i);
		check(found->data.key == d->data.key, "Got the wrong result: %p:%u looking for %u at %u",
				found, found->data.key, d->data.key, i);
	}

	return 1;
error:
	return 0;
}

// test for big number of elements
static char *test_operations()
{
	size_t N = 200;

	RadixMap *map = RadixMap_create(N);
	mu_assert(map != NULL, "Failed to make the map.");
	mu_assert(make_random(map), "Didn't make a random fake radix map.");
	
	RadixMap_sort(map);
	mu_assert(check_order(map), "Failed to properly sort the RadixMap.");

	mu_assert(test_search(map), "Failed the search test.");
	mu_assert(check_order(map), "RadixMap didn't stay sorted after search.");

	while(map->end > 0) {
		RMElement *el = RadixMap_find(map, map->contents[map->end / 2].data.key);
		mu_assert(el != NULL, "Should get a result.");

		size_t old_end = map->end;
		
		mu_assert(RadixMap_delete(map, el) == 0, "Didn't delete it.");
		mu_assert(old_end - 1 == map->end, "Wrong size after delete.");

		// test that the end is now the old value, but uint32 max so it trails off
		mu_assert(check_order(map), "RadixMap didn't stay sorted after delete.");
	}

	RadixMap_destroy(map);

	return NULL;
}

char *test_radixmap_add_perfomance()
{
	struct timespec start, end;
	double diff;
	
	int i = 0, j = 0;
	RadixMap *maps[MAPS_COUNT] = {NULL};

	for(i = 0; i < MAPS_COUNT; i++) {
		maps[i] = RadixMap_create(MAP_SIZE);
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

	for(i = 0; i < MAPS_COUNT; i++) {
	
		for(j = 0; j < maps[i]->max - 1; j++) {
			uint32_t key = (uint32_t)(rand() | rand() << 16);
			RadixMap_add(maps[i], key, j);
		}
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

	diff = (double)get_diff(start, end) / MAPS_COUNT / (MAP_SIZE - 1);
	printf("\nRadixMap add took %lf nanoseconds to run.\n\n", diff);

	for(i = 0; i < MAPS_COUNT; i++) {
		mu_assert(check_order(maps[i]), "Failed to properly sort the RadixMap.");
		RadixMap_destroy(maps[i]);
	}

	return NULL;
}

char *test_radixmap_add_optimized_perfomance()
{
	struct timespec start, end;
	double diff;
	
	int i = 0, j = 0;
	RadixMap *maps[MAPS_COUNT] = {NULL};

	for(i = 0; i < MAPS_COUNT; i++) {
		maps[i] = RadixMap_create(MAP_SIZE);
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

	for(i = 0; i < MAPS_COUNT; i++) {
	
		for(j = 0; j < maps[i]->max - 1; j++) {
			uint32_t key = (uint32_t)(rand() | rand() << 16);
			RadixMap_add_optimized(maps[i], key, j);
		}
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

	diff = (double)get_diff(start, end) / MAPS_COUNT / (MAP_SIZE - 1);
	printf("\nRadixMap add optimized took %lf nanoseconds to run.\n\n", diff);

	for(i = 0; i < MAPS_COUNT; i++) {
		mu_assert(check_order(maps[i]), "Failed to properly sort the RadixMap.");
		RadixMap_destroy(maps[i]);
	}

	return NULL;
}

char *all_tests()
{
	mu_suite_start();
	srand(time(NULL));

	mu_run_test(test_operations);

	mu_run_test(test_radixmap_add_perfomance);
	mu_run_test(test_radixmap_add_optimized_perfomance);

	return NULL;
}

RUN_TESTS(all_tests);
