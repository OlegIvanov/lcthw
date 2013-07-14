#include <stdio.h>

int main(int argc, char *argv[])
{
	int i = 0;

	// go through each string in argv
	// Why am I skipping argv[0]?
	// Initialize i to 0 and see what that does.
	for(i = 0; i < argc; i++) {
		printf("arg %d: %s\n", i, argv[i]);
	}
	/*
	for(i = 1; i < argc; i++) {
		printf("arg %d: %s\n", i, argv[i]);
	}
	*/

	// let's make our own array of strings
	char *states[] = {
		"California", "Oregon",
		"Washington", "Texas"
	};

	// Set num_states wrong so that it's a higher value and see what it does.
	//int num_states = 4;
	int num_states = 10;
	
	for(i = 0; i < num_states; i++) {
		printf("state %d: %s\n", i, states[i]);
	}

	return 0;
}
