// Change the main to conduct some speed tests to see which one is really the fastest.

#include <stdio.h>
#include <string.h>
#include "dbg.h"
#include <time.h>

#define SIZE 1000000

#define BILLION 1E9

int normal_copy(char *from, char *to, int count)
{
	int i = 0;

	for(i = 0; i < count; i++) {
		to[i] = from[i];
	}

	return i;
}

int duffs_device(char *from, char *to, int count)
{
	{
		int n = (count + 7) / 8;
		
		switch(count % 8) {
			case 0: do { *to++ = *from++;
						case 7: *to++ = *from++;
						case 6: *to++ = *from++;
						case 5: *to++ = *from++;
						case 4: *to++ = *from++;
						case 3: *to++ = *from++;
						case 2: *to++ = *from++;
						case 1: *to++ = *from++;				
					} while(--n > 0);
		}
	}

	return count;
}

int zeds_device(char *from, char *to, int count)
{
	{
		int n = (count + 7) / 8;
		
		switch(count % 8) {
			case 0:
			again: *to++ = *from++;
	
			case 7: *to++ = *from++;
			case 6: *to++ = *from++;
			case 5: *to++ = *from++;
			case 4: *to++ = *from++;
			case 3: *to++ = *from++;
			case 2: *to++ = *from++;
			case 1: *to++ = *from++;
					if(--n > 0) goto again;
		}
	}

	return count;
}

int valid_copy(char *data, int count, char expects)
{
	int i = 0;
	for(i = 0; i < count; i++) {
		if(data[i] != expects) {
			log_err("[%d] %c != %c", i, data[i], expects);
			return 0;
		}
	}

	return 1;
}

int main(int argc, char *argv[])
{
	char from[SIZE] = {'a'};
	char to[SIZE] = {'c'};
	int rc = 0;
	int i = 0;

	struct timespec start, end;
	double diff;

	// setup the from to have some stuff
	memset(from, 'x', SIZE);
	// set it to a failure mode;
	memset(to, 'y', SIZE);
	check(valid_copy(to, SIZE, 'y'), "Not initialized right.");

	// use normal copy to
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	for(i = 0; i < 100; i++) {
		rc = normal_copy(from, to, SIZE);
	} 
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

	diff = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec ) / BILLION;

	log_info("Normal copy took %lf seconds to run.", diff);

	check(rc == SIZE, "Normal copy failed: %d", rc);
	check(valid_copy(to, SIZE, 'x'), "Normal copy failed.");

	// reset
	memset(to, 'y', SIZE);

	// duffs version
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	for(i = 0; i < 100; i++) {
		rc = duffs_device(from, to, SIZE);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

	diff = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec ) / BILLION;

	log_info("Duff copy took %lf seconds to run.", diff);

	check(rc == SIZE, "Duff's device failed: %d", rc);
	check(valid_copy(to, SIZE, 'x'), "Duff's device failed copy.");

	// reset
	memset(to, 'y', SIZE);

	// my version
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	for(i = 0; i < 100; i++) {
		rc = zeds_device(from, to, SIZE);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

	diff = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec ) / BILLION;

	log_info("Zed copy took %lf seconds to run.", diff);
	
	check(rc == SIZE, "Zed's device failed: %d", rc);
	check(valid_copy(to, SIZE, 'x'), "Zed's device failed copy.");
	
	return 0;
error:
	return 1;
}
