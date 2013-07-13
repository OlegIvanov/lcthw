#include <stdio.h>

int main(int argc, char *argv[])
{
	long universe_of_detects = 
		1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 7L + 
		(1024L * 1024L * 1024L * 1024L * 1024L * 1024L - 1L);

	printf("Extra Credit 1.\n");
	printf("Max universe_of_detects value: %ld.\n",
			universe_of_detects);

	unsigned long unsigned_universe_of_detects = 
		1024UL * 1024UL * 1024UL * 1024UL * 1024UL * 1024UL * 15UL +
		(1024UL * 1024UL * 1024UL * 1024UL * 1024UL * 1024UL - 1UL);
	
	printf("Extra Credit 3.\n");
	printf("Max unsigned_universe_of_detects value: %lu.\n", 
			unsigned_universe_of_detects);

	return 0;
}
