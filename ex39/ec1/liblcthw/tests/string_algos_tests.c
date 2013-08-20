#include "minunit.h"
#include <lcthw/string_algos.h>
#include <lcthw/bstrlib.h>
#include <time.h>

struct tagbstring IN_STR = bsStatic("I have ALPHA beta ALPHA and oranges ALPHA");
struct tagbstring ALPHA = bsStatic("ALPHA");
struct tagbstring have = bsStatic("have");
const int TEST_TIME = 9;

#define RANDOM_STRING_COUNT 100000

char *test_randomize_string()
{
	struct bstrList *random_strings = bsplit(&IN_STR, 'a');

	int i = 0;

	for(i = 0; i < random_strings->qty; i++) {
		debug("%s", bdata(random_strings->entry[i]));
	}

	bstrListDestroy(random_strings);
	
	return NULL;
}

char *test_find_and_scan()
{
	StringScanner *scan = StringScanner_create(&IN_STR);
	mu_assert(scan != NULL, "Failed to make the scanner.");

	int find_i = String_find(&IN_STR, &ALPHA, 1);
	mu_assert(find_i > 0, "Failed to find 'ALPHA' in test string.");

	int scan_i = StringScanner_scan(scan, &ALPHA);
	mu_assert(scan_i > 0, "Failed to find 'ALPHA' with scan.");
	mu_assert(scan_i == find_i, "find and scan don't match");

	scan_i = StringScanner_scan(scan, &ALPHA);
	mu_assert(scan_i > find_i, "should find another ALPHA after the first");

	scan_i = StringScanner_scan(scan, &ALPHA);
	mu_assert(scan_i > find_i, "should find another ALPHA after the first");

	mu_assert(StringScanner_scan(scan, &ALPHA) == -1, "shouldn't find it");

	StringScanner_destroy(scan);

	return NULL;
}

#define SHORT_BURST

char *test_binstr_perfomance()
{
	int i = 0;
	int found_at = 0;
	unsigned long find_count = 0;
	time_t elapsed = 0;

#ifdef SHORT_BURST
	debug("short burst");
	for(i = 0; i < 1000; i++) {
		found_at = binstr(&IN_STR, 0, &ALPHA);
	}
#endif

	time_t start = time(NULL);

	do {
		for(i = 0; i < 1000; i++) {
			found_at = binstr(&IN_STR, 0, &ALPHA);
			mu_assert(found_at != BSTR_ERR, "Failed to find!");
			find_count++;
		}

		elapsed = time(NULL) - start;
	} while(elapsed <= TEST_TIME);

	debug("BINSTR COUNT: %lu, END TIME: %d, OPS: %f",
			find_count, (int)elapsed, (double)find_count / elapsed);
	return NULL;
}

char *test_find_perfomance()
{
	int i = 0;
	int found_at = 0;
	unsigned long find_count = 0;
	time_t elapsed = 0;

#ifdef SHORT_BURST
	debug("short burst");
	for(i = 0; i < 1000; i++) {
		found_at = String_find(&IN_STR, &ALPHA, 0);
	}
#endif

	time_t start = time(NULL);

	do {
		for(i = 0; i < 1000; i++) {
			found_at = String_find(&IN_STR, &ALPHA, 0);
			find_count++;
		}
	
		elapsed = time(NULL) - start;
	} while(elapsed <= TEST_TIME);

	debug("FIND COUNT: %lu, END TIME: %d, OPS: %f",
			find_count, (int)elapsed, (double)find_count / elapsed);

	return NULL;
}

char *test_scan_perfomance()
{
	int i = 0;
	int found_at = 0;
	unsigned long find_count = 0;
	time_t elapsed = 0;
	StringScanner *scan = StringScanner_create(&IN_STR);

#ifdef SHORT_BURST
	debug("short burst");
	for(i = 0; i < 1000; i++) {
		found_at = StringScanner_scan(scan, &ALPHA);
	}
#endif

	time_t start = time(NULL);

	do {
		for(i = 0; i < 1000; i++) {
			found_at = 0;

			do {
				found_at = StringScanner_scan(scan, &ALPHA);
				find_count++;
			} while(found_at != -1);
		}

		elapsed = time(NULL) - start;
	} while(elapsed <= TEST_TIME);

	debug("SCAN COUNT: %lu, END TIME: %d, OPS: %f",
			find_count, (int)elapsed, (double)find_count / elapsed);

	StringScanner_destroy(scan);

	return NULL;
}

char *test_find_improved()
{
	StringScanner *scan = StringScanner_create(&IN_STR);
	mu_assert(scan != NULL, "Failed to make the scanner.");

	// test 1
	int scan_i = StringScanner_scan(scan, &ALPHA);
	debug("scan_i: %d", scan_i);
	mu_assert(scan_i > 0, "Failed to find 'ALPHA' with scan.");

	int find_i = String_find(&IN_STR, &ALPHA, 1);
	debug("find_i: %d", find_i);
	mu_assert(find_i == scan_i, "find and scan don't match");

	// test 2
	scan_i = StringScanner_scan(scan, &ALPHA);
	debug("scan_i: %d", scan_i);
	mu_assert(scan_i > 0, "Failed to find 'ALPHA' with scan.");

	find_i = String_find(&IN_STR, &ALPHA, 0);
	debug("find_i: %d", find_i);
	mu_assert(find_i == scan_i, "find and scan don't match");

	// test 3
	scan_i = StringScanner_scan(scan, &ALPHA);
	debug("scan_i: %d", scan_i);
	mu_assert(scan_i > 0, "Failed to find 'ALPHA' with scan.");

	find_i = String_find(&IN_STR, &ALPHA, 0);
	debug("find_i: %d", find_i);
	mu_assert(find_i == scan_i, "find and scan don't match");

	// test 4
	scan_i = StringScanner_scan(scan, &ALPHA);
	debug("scan_i: %d", scan_i);
	mu_assert(scan_i == -1, "shouldn't find it");

	find_i = String_find(&IN_STR, &ALPHA, 0);
	debug("find_i: %d", find_i);
	mu_assert(find_i == -1, "shouldn't find it");

	// test 5
	scan_i = StringScanner_scan(scan, &ALPHA);
	debug("scan_i: %d", scan_i);
	mu_assert(scan_i > 0, "Failed to find 'ALPHA' with scan.");

	find_i = String_find(&IN_STR, &ALPHA, 0);
	debug("find_i: %d", find_i);
	mu_assert(find_i == scan_i, "find and scan don't match");

	// test 6
	find_i = String_find(&IN_STR, &have, 0);
	debug("find_i: %d", find_i);
	mu_assert(find_i == 2, "find works right even if we don't reset it");

	// test 7
	StringScanner_destroy(scan);

	return NULL;
}

char *all_tests()
{
	mu_suite_start();

	mu_run_test(test_find_and_scan);
	mu_run_test(test_find_improved);

	// this is an idiom for commenting out sections of code
#if 0
	mu_run_test(test_scan_perfomance);
	mu_run_test(test_find_perfomance);
	mu_run_test(test_binstr_perfomance);
#endif

	mu_run_test(test_randomize_string);

	return NULL;
}

RUN_TESTS(all_tests);
