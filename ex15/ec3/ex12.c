#include <stdio.h>

// Go back to some of the other programs that use arrays and try to use pointers instead.
int main(int argc, char *argv[])
{
	int i = 0;

	if (argc == 1) {
		printf("You only have one argument. You suck.\n");
	} else if(argc > 1 && argc < 4) {
		printf("Here's your arguments:\n");
		for(i = 0; i < argc; i++) {
			printf("%s ", *(argv + i));
		}
		printf("\n");
	} else {
		printf("You have too many arguments. You suck.\n");
	}

	return 0;
}
