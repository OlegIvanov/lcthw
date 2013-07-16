#include <stdio.h>

// Process command line arguments using just pointers similar to how you did names in this one.
int main(int argc, char *argv[])
{
	char** args = argv;
	int i = 0;

	for(i = 0; i < argc; i++) {
		printf("argument %d: %s\n", i, *(args + i));
	}

	return 0;
}
