#include <stdio.h>

// Go back to some of the other programs that use arrays and try to use pointers instead.
int main(int argc, char *argv[])
{
	int numbers[4] = {0};
	char name[4] = {'a'};

	// first, print them out raw
	printf("numbers: %d %d %d %d\n",
			*numbers, *(numbers + 1),
			*(numbers + 2), *(numbers + 3));

	printf("name each: %c %c %c %c\n",
			*name, *(name + 1),
			*(name + 2), *(name + 3));

	printf("name: %s\n", name);

	// setup the numbers
	*numbers = 1;
	*(numbers + 1) = 2;
	*(numbers + 2) = 3;
	*(numbers + 3) = 4;

	// setup the name
	*name = 'Z';
	*(name + 1) = 'e';
	*(name + 2) = 'd';
	*(name + 3) = '\0';

	// then print them out initialized
	printf("numbers: %d %d %d %d\n",
			*numbers, *(numbers + 1),
			*(numbers + 2), *(numbers + 3));

	printf("name each: %c %c %c %c\n",
			*name, *(name + 1),
			*(name + 2), *(name + 3));

	// print the name like a string
	printf("name: %s\n", name);
	
	// another way to use name
	char *another = "Zed";
	
	printf("another: %s\n", another);
	
	printf("another each: %c %c %c %c\n",
			*another, *(another + 1),
			*(another + 2), *(another + 3));

	return 0;
}
