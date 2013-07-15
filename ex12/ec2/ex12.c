#include <stdio.h>

int main(int argc, char *argv[])
{
	// Is the first test really saying the right thing? To you the "first argument" isn't the same first argument a user entered. Fix it.
	int i = 0;
	argc--;

	if (argc == 0) {
		printf("You just simply have no arguments.\n");
	} else if (argc == 1) {
		printf("You only have one argument. You suck.\n");
	} else if(argc > 1 && argc < 4) {
		printf("Here's your arguments:\n");
		for(i = 1; i < argc + 1; i++) {
			printf("%s ", argv[i]);
		}
		printf("\n");
	} else {
		printf("You have too many arguments. You suck.\n");
	}

	return 0;
}
