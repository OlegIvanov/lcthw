// Rewrite this to not use fscanf at all. 
// You'll need to use functions like atoi to convert the input strings to numbers.

#include <stdio.h>
#include "dbg.h"
#include <stdlib.h>

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

int main(int argc, char *argv[])
{
	Person you = {.age = 0};
	int i = 0;
	char *in = NULL;

	char buff[MAX_DATA];

	printf("What's your First Name? ");
	in = fgets(you.first_name, MAX_DATA - 1, stdin);
	check(in != NULL, "Failed to read first name.");

	printf("What's your Last Name? ");
	in = fgets(you.last_name, MAX_DATA - 1, stdin);
	check(in != NULL, "Failed to read last name.");

	printf("How old are you? ");
	in = fgets(buff, MAX_DATA - 1, stdin);
	check(in != NULL, "Failed to read age.");
	you.age = atoi(buff);
	
	printf("What colour are your eyes:\n");
	for(i = 0; i <= OTHER_EYES; i++) {
		printf("%d) %s\n", i + 1, EYE_COLOR_NAMES[i]);
	}
	printf("> ");
	
	int eyes = -1;
	in = fgets(buff, MAX_DATA - 1, stdin);
	check(in != NULL, "Failed to read eyes.");
	eyes = atoi(buff);

	you.eyes = eyes - 1;
	check(you.eyes <= OTHER_EYES && you.eyes >= 0, "Do it right, that's not an option.");

	printf("How much do you make an hour? ");
	in = fgets(buff, MAX_DATA - 1, stdin);
	check(in != NULL, "Failed to read income.");
	you.income = atof(buff);

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
