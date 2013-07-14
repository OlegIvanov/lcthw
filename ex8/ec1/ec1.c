#include <stdio.h>

int main(int argc, char *argv[])
{
	int areas[] = {10, 12, 13, 14, 20};
	char name[] = "Zed";
	char full_name[] = {
		'Z', 'e', 'd',
		' ', 'A', '.', ' ',
		'S', 'h', 'a', 'w', '\0'
	};
	
	// WARNING: On some systems you may have to change the
	// %ld in this code to a %u since it will use unsigned ints
	printf("The size of an int: %ld\n", sizeof(int));
	printf("The size of areas (int[]): %ld\n", 
			sizeof(areas));
	printf("The number of ints in areas: %ld\n",
			sizeof(areas) / sizeof(int));
	printf("The first area is %d, the 2nd %d.\n",
			areas[0], areas[1]);
	
	printf("The size of a char: %ld\n", sizeof(char));
	printf("The size of name (char[]): %ld\n",
			sizeof(name));
	printf("The number of chars: %ld\n",
			sizeof(name) / sizeof(char));
	
	printf("The size of full_name (char[]): %ld\n",
			sizeof(full_name));
	printf("The number of chars: %ld\n",
			sizeof(full_name) / sizeof(char));

	printf("name=\"%s\" and full_name=\"%s\"\n", 
			name, full_name);

	printf("Try assigning to elements in the areas array with areas[0] = 100; and similar.\n");
	areas[0] = 65;
	printf("areas[0]: %d.\n", areas[0]);
	areas[4] = 777;
	printf("areas[4]: %d.\n", areas[4]);
	areas[5] = 6666;
	printf("areas[5]: %d.\n", areas[5]);
	
	printf("Try assigning to elements of name and full_name.\n");
	name[0] = 'T';
	printf("name[0] = 'T': name=%s\n", name);
	name[3] = 'X';
	printf("name[3] = 'X': name=%s\n", name);

	full_name[2] = '\0';
	printf("full_name[2] = '\\0': full_name=%s\n", full_name);

	printf("Try setting one element of areas to a character from name.\n");
	name[0] = areas[0];
	printf("name[0] = areas[0]: name=%s\n", name);

	return 0;
}
