#include <stdio.h>

// Rewrite this program to use a function for each of the ways you're printing out things. 
// Try to pass pointers to these functions so they work on the data. 
// Remember you can declare a function to accept a pointer, but just use it like an array.
int first_way_using_indexing(char**, int*, int);
int second_way_using_pointers(char**, int*, int);
int third_way_pointers_are_just_arrays(char**, int*, int);
int fourth_way_with_pointers_in_a_stupid_complex_way(char**, int*, int);

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
	
	first_way_using_indexing(names, ages, count);
	printf("---\n");

	// setup the pointers to the start of the arrays
	int *cur_age = ages;
	char **cur_name = names;

	second_way_using_pointers(cur_name, cur_age, count);
	printf("---\n");
	
	third_way_pointers_are_just_arrays(cur_name, cur_age, count);
	printf("---\n");
		
	fourth_way_with_pointers_in_a_stupid_complex_way(cur_name, cur_age, count);

	return 0;
}

int first_way_using_indexing(char** names, int* ages, int count)
{
	int i = 0;

	for(i = 0; i < count; i++) {
		printf("%s has %d years alive.\n",
				names[i], ages[i]);
	}

	return 0;
}

int second_way_using_pointers(char** names, int* ages, int count)
{
	int i = 0;
	
	for(i = 0; i < count; i++) {
		printf("%s is %d years old.\n", 
				*(names + i), *(ages + i));
	}

	return 0;
}

int third_way_pointers_are_just_arrays(char** names, int* ages, int count)
{
	int i = 0;
	
	for(i = 0; i < count; i++) {
		printf("%s is %d years old again.\n", 
				names[i], ages[i]);
	}

	return 0;
}

int fourth_way_with_pointers_in_a_stupid_complex_way(char** names, int* ages, int count)
{
	char** cur_name = names;
	int* cur_age = ages;

	for(cur_name = names, cur_age = ages;
		(cur_age - ages) < count;
		cur_name++, cur_age++)
	
		printf("%s lived %d years so far.\n",
				*cur_name, *cur_age);
	
	return 0;
}
