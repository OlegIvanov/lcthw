#include <stdio.h>

// Change the for-loops to while-loops and see what works better for which kind of pointer usage.
int main(int argc, char *argv[])
{
	// create two arrays we care about
	int ages[] = {23, 43, 12, 89, 2};
	char *names[] = {
		"Alan", "Frank",
		"Mary", "John", "Lisa"
	};
	
	// safely get the size of ages
	int count = sizeof(ages) / sizeof(int);
	int i = 0;
	
	// first way using indexing
	while(i < count) {
		printf("%s has %d years alive.\n",
				names[i], ages[i]);
		i++;
	}

	printf("---\n");

	// setup the pointers to the start of the arrays
	int *cur_age = ages;
	char **cur_name = names;

	// second way using pointers
	i = 0;
	while(i < count) {
		printf("%s is %d years old.\n", 
				*(cur_name + i), *(cur_age + i));
		i++;
	}

	printf("---\n");

	// third way, pointers are just arrays
	i = 0;
	while(i < count) {
		printf("%s is %d years old again.\n", 
				cur_name[i], cur_age[i]);
		i++;
	}

	printf("---\n");

	// fourth way with pointers in a stupid complex way
	cur_name = names;
	cur_age = ages;
	while((cur_age - ages) < count) {
		printf("%s lived %d years so far.\n",
				*cur_name, *cur_age);
		cur_name++;
		cur_age++;
	}

	return 0;
}
