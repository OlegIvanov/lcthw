#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Have print_arguments figure how long each argument string is using the strlen function, and then pass that length to print_letters. 
// Then, rewrite print_letters so it only processes this fixed length and doesn't rely on the '\0' terminator. 
// You will need the #include <string.h> for this.

// forward declarations
int can_print_it(char ch);
void print_letters(char arg[]);

void print_arguments(int argc, char *argv[])
{
	int i = 0;
	
	for(i = 0; i < argc; i++) {
		print_letters(argv[i]);
	}
}

void print_letters(char arg[]) 
{
	int i = 0;
	int arg_len = strlen(arg);
	for(i = 0; i < arg_len; i++) {
		char ch = arg[i];

		if(can_print_it(ch)) {
			printf("'%c' == %d ", ch, ch);
		}
	}

	printf("\n");
}

int can_print_it(char ch) 
{
	return isalpha(ch) || isblank(ch);
}

int main(int argc, char *argv[])
{
	print_arguments(argc, argv);
	return 0;
}
