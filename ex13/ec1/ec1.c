#include <stdio.h>

char to_lowercase(char);

int main(int argc, char *argv[])
{
	if(argc == 1) {
		printf("You have no arguments.\n");
		return 1;
	}

	int i, j;
	char letter;
	// Make it handle all of the arguments you pass it with yet another for-loop.
	for(i = 1; i < argc; i++)
	{
		printf("Unchanged argument %d: %s\n", i, argv[i]);
		// Use the ',' (comma) to initialize letter in the for-loop.
		for(j = 0, letter = argv[i][j]; argv[i][j] != '\0'; j++, letter = argv[i][j]) {
			switch(letter) {
				case 'a':
				case 'A':
					printf("%d: 'A'\n", j);
					break;
				case 'e':
				case 'E':
					printf("%d: 'E'\n", j);
					break;
				case 'i':
				case 'I':
					printf("%d: 'I'\n", j);
					break;
				case 'o':
				case 'O':
					printf("%d: 'O'\n", j);
					break;
				case 'u':
				case 'U':
					printf("%d: 'U'\n", j);
					break;
				case 'y':
				case 'Y':
					if(j > 2 ) {
						// it's only sometimes Y
						printf("%d: 'Y'\n", j);
						break;
					}
				default:
					if(letter >= 65 && letter <= 90)
						printf("%d: %c is extraneous symbol that has been successfully turned to lowercase\n", j, to_lowercase(letter));
					else if(letter >= 97 && letter <= 122)
						printf("%d: %c is extraneous symbol that is already lowercase (nothing to be done)\n", j, letter);
					else
						printf("%d: %c is nonalphabetical symbol (nothing to be done)\n", j, letter);
			}
		}
	}

	return 0;
}

char to_lowercase(char letter)
{
	char lowercase_letter = letter;

	if(letter >= 65 && letter <= 90)
		lowercase_letter += 32;

	return lowercase_letter;
}
