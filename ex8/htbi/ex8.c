#include <stdio.h>

int main(int argc, char *argv[])
{
	char full_name[] = {
		'Z', 'e', 'd',
		' ', 'A', '.', ' ',
		'S', 'h', 'a', 'w'
	};
	int areas[] = {10, 12, 13, 14, 20};
	char name[] = "Zed";
	
	// WARNING: On some systems you may have to change the
	// %ld in this code to a %u since it will use unsigned ints
	printf("The size of an int: %ld\n", sizeof(int));
	printf("The size of areas (int[]): %ld\n", 
			sizeof(areas));
	printf("The number of ints in areas: %ld\n",
			sizeof(areas) / sizeof(int));
	
	printf("Change it so that instead of areas[0] you try to print areas[10] and see what Valgrind thinks of that.\n");
	printf("The first area is %d, the 2nd %d.\n",
			areas[10], areas[1]);
	
	printf("The size of a char: %ld\n", sizeof(char));
	printf("The size of name (char[]): %ld\n",
			sizeof(name));
	printf("The number of chars: %ld\n",
			sizeof(name) / sizeof(char));
	
	printf("Get rid of the '\\0' at the end of full_name.\n");
	printf("Move the definition of full_name to the top of main before areas.\n");

	printf("The size of full_name (char[]): %ld\n",
			sizeof(full_name));
	printf("The number of chars: %ld\n",
			sizeof(full_name) / sizeof(char));
	
	printf("name=\"%s\" and full_name=\"%s\"\n", 
			name, full_name);

	printf("Try other versions of these, doing it to name and full_name too.\n");
	printf("name[10](%%c): %c.\n", name[10]);
	printf("name[10](%%d): %d.\n", name[10]);
	printf("full_name[11](%%c): %c.\n", full_name[11]);
	printf("full_name[11](%%s): %s.\n", full_name[11]);
	printf("full_name[4](%%c): %c.\n", full_name[4]);
	printf("full_name[10](%%c): %c.\n", full_name[10]);

	return 0;
}
