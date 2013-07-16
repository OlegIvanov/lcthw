#include <stdio.h>

int main(int argc, char *argv[])
{
	// create two arrays we care about
	int ages[] = {23, 43, 12, 89, 2};
	char *names[] = {
		"Alan", "Frank",
		"Mary", "John", "Lisa"
	};

	// Try rewriting the loops so they start at the end of the arrays and go to the beginning. 
	// This is harder than it looks.
	
	// safely get the size of ages
	int count = sizeof(ages) / sizeof(int);
	int i = 0;
	
	// first way using indexing
	for(i = count; i > 0; i--) {
		printf("%s has %d years alive.\n",
				names[count - i], ages[count - i]);
	}

	printf("---\n");

	// setup the pointers to the start of the arrays
	int *cur_age = ages;
	char **cur_name = names;

	// second way using pointers
	for(i = count; i > 0; i--) {
		printf("%s is %d years old.\n", 
				*(cur_name + count - i), *(cur_age + count - i));
	}

	printf("---\n");

	// third way, pointers are just arrays
	for(i = count; i > 0; i--) {
		printf("%s is %d years old again.\n", 
				cur_name[count - i], cur_age[count - i]);
	}

	printf("---\n");

	// fourth way with pointers in a stupid complex way
	for(cur_name = names + count, cur_age = ages + count;
			(cur_age - ages) > 0;
			cur_name--, cur_age--)
	{
		printf("%s lived %d years so far.\n",
				*(cur_name - (cur_age - ages) + count - (cur_age - ages)),
				*(cur_age - (cur_age - ages) + count - (cur_age - ages)));
	}

	return 0;
}
