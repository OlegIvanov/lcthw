#include <stdio.h>

int main(int argc, char *argv[])
{
	int i = 0;
	
	// Write a few more test cases for this program to see what you can come up with.
	if (argc == 1) {
		printf("You only have one argument. You suck.\n");
	} 
	else if(argc > 1 && argc < 4) {
		if(argc != 3) {
			printf("I am so relieved!\n");
		}
		printf("Here's your arguments:\n");
		for(i = 0; i < argc; i++) {
			printf("%s ", argv[i]);
		}
		printf("\n");
	} else {
		printf("You have too many arguments. You suck.\n");
	}
	
	if (argc % 2 == 1)
		printf("Piss coming from my ass. Suck it, bitch.\n");

	return 0;
}
