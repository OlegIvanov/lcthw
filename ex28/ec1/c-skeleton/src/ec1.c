#include <stdio.h>
#include <string.h>
#include "dbg.h"
#include "ec1.h"

#define SIZE 20

int hello_world()
{
	printf("Hello world.\n");
	
	return 0;
}

int bad_function_test()
{
	char src[SIZE] = "Hello world.";
	char dest[SIZE];
	
	char *res = strncpy(dest, src, SIZE);
	check(res, "Shouldn't be NULL.");

	return 0;

error:
	return -1;
}
