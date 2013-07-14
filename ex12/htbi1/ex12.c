#include <stdio.h>

int main(int argc, char *argv[])
{
	int i = 0;

	if (argc == 1) {
		printf("You only have one argument. You suck.\n");
	}
	// Change the && to a || so you get an "or" instead of "and" test and see how that works.
	// else if(argc > 1 && argc < 4) {
	else if(argc > 1 || argc < 4) {
		printf("Here's your arguments:\n");
		for(i = 0; i < argc; i++) {
			printf("%s ", argv[i]);
		}
		printf("\n");
	} 
	// Remove the else at the end and it won't catch the edge case.
	/*else {
		printf("You have too many arguments. You suck.\n");
	}*/

	return 0;
}
