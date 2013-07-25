// Fix it so that the input names get stripped of the trailing newline characters and any whitespace.

#include <stdio.h>
#include <ctype.h>
#include "dbg.h"

#define MAX_DATA 100

typedef enum EyeColor {
	BLUE_EYES, GREEN_EYES, BROWN_EYES,
	BLACK_EYES, OTHER_EYES
} EyeColor;

const char *EYE_COLOR_NAMES[] = {
	"Blue", "Green", "Brown", "Black", "Other"
};

typedef struct Person {
	int age;
	char first_name[MAX_DATA];
	char last_name[MAX_DATA];
	EyeColor eyes;
	float income;
} Person;

void remove_whitespaces_newline_characters(char *input, int length)
{
	int i, j = 0;
	
	check(length == 0, "Input length = 0");

	for(i = 0; i < length; i++)	{
		char c = input[i];
		
		if(isblank(c) || c == '\n') continue;
		if(c == '\0') break;

		input[j++] = c;
	}

	int terminate_index = (j == length) ? j - 1 : j;
	input[terminate_index] = '\0';

error:
	return;
}

int main(int argc, char *argv[])
{
	Person you = {.age = 0};
	int i = 0;
	char *in = NULL;

	printf("What's your First Name? ");
	in = fgets(you.first_name, MAX_DATA - 1, stdin);
	check(in != NULL, "Failed to read first name.");
	remove_whitespaces_newline_characters(you.first_name, MAX_DATA);

	printf("What's your Last Name? ");
	in = fgets(you.last_name, MAX_DATA - 1, stdin);
	check(in != NULL, "Failed to read last name.");
	remove_whitespaces_newline_characters(you.last_name, MAX_DATA);

	printf("How old are you? ");
	int rc = fscanf(stdin, "%d", &you.age);
	check(rc > 0, "You have to enter a number.");
	
	printf("What colour are your eyes:\n");
	for(i = 0; i <= OTHER_EYES; i++) {
		printf("%d) %s\n", i + 1, EYE_COLOR_NAMES[i]);
	}
	printf("> ");
	
	int eyes = -1;
	rc = fscanf(stdin, "%d", &eyes);
	check(rc > 0, "You have to enter a number.");

	you.eyes = eyes - 1;
	check(you.eyes <= OTHER_EYES && you.eyes >= 0, "Do it right, that's not an option.");

	printf("How much do you make an hour? ");
	rc = fscanf(stdin, "%f", &you.income);
	check(rc > 0, "Enter a floating point number.");

	printf("----- RESULTS -----\n");
	
	printf("First Name: %s", you.first_name);
	printf("Last Name: %s", you.last_name);
	printf("Age: %d\n", you.age);
	printf("Eyes: %s\n", EYE_COLOR_NAMES[you.eyes]);
	printf("Income: %f\n", you.income);
	
	return 0;
error:

	return -1;
}
