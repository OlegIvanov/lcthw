#include <stdio.h>

int main(int argc, char *argv[])
{
	// go through each string in argv

	// Make these loops count backward by using i-- to start at argc and count down to 0. You may have to do some math to 		
	// make the array indexes work right.
	/*
	int i = argc;
	while(i > 0) {
		printf("arg %d: %s\n", argc - i, argv[argc - i]);
		i--;
	}
	
	// let's make our own array of strings
	char *states[] = {
		"Califirnia", "Oregon",
		"Washington", "Texas"
	};
	
	int num_states = 4;
	i = num_states; // watch for this
	while(i > 0) {
		printf("state %d: %s\n", num_states - i, states[num_states - i]);
		i--;
	}
	*/

	int i = argc;
	while(i > 0) {
		printf("arg %d: %s\n", argc - i, argv[argc - i]);
		i--;
	}
	
	// let's make our own array of strings
	char *states[] = {
		"Califirnia", "Oregon",
		"Washington", "Texas"
	};
	
	int num_states = 4;
	i = num_states; // watch for this
	while(i > 0) {
		printf("state %d: %s\n", num_states - i, states[num_states - i]);
		i--;
	}
	
	printf("Use a while loop to copy the values from argv into states.\n");
	printf("Make this copy loop never fail such that if there's too many argv elements it won't put them all into states.\n");

	i = 0;
	while(i < argc) {
		states[i] = argv[i];
		printf("state %d: %s\n", i, states[i]);
		i++;
		
		if (i == num_states)
			break;
	}
	
	return 0;
}
