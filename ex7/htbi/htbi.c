#include <stdio.h>

int main(int argc, char *argv[])
{
	char nul_byte = '\0';

	printf("How To Break It.\n");
	printf("Printing out nul_byte (%%s): %s.\n", nul_byte);
	printf("Printing out nul_byte (%%c): %c.\n", nul_byte);

	return 0;
}
