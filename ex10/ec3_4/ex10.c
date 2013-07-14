#include <stdio.h>

int main(int argc, char *argv[])
{
	int i = 0;

	// let's make our own array of strings
	char *states[] = {
		"California", "Oregon",
		"Washington", "Texas"
	};

	// See if you can assign an element from the states array to the argv array before printing both.
	argv[1] = states[0];
	// Try the inverse.
	states[0] = argv[0];
	
	// go through each string in argv
	// Why am I skipping argv[0]?
	for(i = 1; i < argc; i++) {
		printf("arg %d: %s\n", i, argv[i]);
	}

	int num_states = 4;

	// Read what a NULL is and try to use it in one of the elements of the states array to see what it'll print.
	states[1] = NULL;
	
	for(i = 0; i < num_states; i++) {
		printf("state %d: %s\n", i, states[i]);
	}

	return 0;
}
