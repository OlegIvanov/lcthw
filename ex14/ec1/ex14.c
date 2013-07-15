#include <stdio.h>
#include <ctype.h>

// Rework these functions so that you have fewer functions.

void print_arguments(int argc, char *argv[])
{
	int i = 0;
	
	for(i = 0; i < argc; i++) {
		int j = 0;
	
		for(j = 0; argv[i][j] != '\0'; j++) {
			char ch = argv[i][j];
			
			if(isalpha(ch) || isblank(ch)) {
				printf("'%c' == %d ", ch, ch);
			}
		}

		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	print_arguments(argc, argv);
	return 0;
}
