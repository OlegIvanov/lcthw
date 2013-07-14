#include <stdio.h>

int main(int argc, char *argv[])
{
	int numbers[4] = {0};
	// Convert name to be in the style of another and see if the code keeps working.
	//char name[4] = {'a'};
	char* name = "a";
	

	// first, print them out raw
	printf("numbers: %d %d %d %d\n",
			numbers[0], numbers[1],
			numbers[2], numbers[3]);

	printf("name each: %c %c %c %c\n",
			name[0], name[1],
			name[2], name[3]);

	printf("name: %s\n", name);

	// setup the numbers
	numbers[0] = 1;
	numbers[1] = 2;
	numbers[2] = 3;
	numbers[3] = 4;

	// setup the name
	name[0] = 'Z';
	name[1] = 'e';
	name[2] = 'd';
	name[3] = '\0';
	
	// then print them out initialized
	printf("numbers: %d %d %d %d\n",
			numbers[0], numbers[1],
			numbers[2], numbers[3]);

	printf("name each: %c %c %c %c\n",
			name[0], name[1],
			name[2], name[3]);

	// print the name like a string
	printf("name: %s\n", name);
	
	// another way to use name
	char *another = "Zed";
	
	printf("another: %s\n", another);
	
	printf("another each: %c %c %c %c\n",
			another[0], another[1],
			another[2], another[3]);
	
	/*
	printf("Extra Credit 1.\n");
	printf("Assign the characters into numbers and then use printf to print them a character at a time.\n");

	numbers[0] = name[0];
	numbers[1] = name[1];
	numbers[2] = name[2];
	numbers[3] = name[3];
	
	printf("numbers: %c %c %c %c\n",
			numbers[0], numbers[1],
			numbers[2], numbers[3]);
	*/
	/*
	printf("Extra Credit 2.\n");
	printf("Do the inverse for name, trying to treat it like an array of int and print it out one int at a time.\n");

	name[0] = numbers[0];
	name[1] = numbers[1];
	name[2] = numbers[2];
	name[3] = numbers[3];

	printf("name: %d %d %d %d\n",
			name[0], name[1],
			name[2], name[3]);
	*/
	/*
	printf("Extra Credit 4.\n");
	printf("If an array of characters is 4 bytes long, and an integer is 4 bytes long, then can you treat the\n");
	printf("whole name array like it's just an integer? How might you accomplish this crazy hack?\n");

	printf("name each (hex): %x %x %x %x\n",
			name[0], name[1],
			name[2], name[3]);

	int* name_as_int_number	= (int*)name;

	printf("name as int number(hex)=%x\n", *name_as_int_number);
	printf("name as int number(decimal)=%d\n", *name_as_int_number);
	*/
	
	return 0;
}
