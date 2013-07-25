#include <stdio.h>
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

int main(int argc, char *argv[])
{
	Person you = {.age = 0};
	int i = 1;
	char *in = NULL;

	// To demonstrate the problems with fscanf and strings, 
	// change the lines that use fgets so they are fscanf(stdin, "%50s", you.first_name) 
	// and then try to use it again. 
	// Notice it seems to read too much and then eat your enter key? 
	// This doesn't do what you think it does, 
	// and really rather than deal with weird scanf issues, just use fgets.
	
	/*
	printf("What's your First Name? ");
	fscanf(stdin, "%50s", you.first_name);

	printf("What's your Last Name? ");
	fscanf(stdin, "%50s", you.last_name);
	*/

	// Next, change the fgets to use gets, then bust out your valgrind and do this: 
	// valgrind ./ex24 < /dev/urandom to feed random garbage into your program.
	
	/*
	printf("What's your First Name? ");
	in = gets(you.first_name);
	check(in != NULL, "Failed to read first name.");
	
	printf("What's your Last Name? ");
	in = gets(you.last_name);
	check(in != NULL, "Failed to read last name.");
	*/
	
	printf("What's your First Name? ");
	in = fgets(you.first_name, MAX_DATA - 1, stdin);
	check(in != NULL, "Failed to read first name.");
	
	printf("What's your Last Name? ");
	in = fgets(you.last_name, MAX_DATA - 1, stdin);
	check(in != NULL, "Failed to read last name.");

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

	// Finally, take the input for you.eyes and remove the check that the number given is within the right range.
	// Then feed it bad numbers like -1 or 1000. Do this under Valgrind too so you can see what happens.

	you.eyes = eyes - 1;
	/*
	check(you.eyes <= OTHER_EYES && you.eyes >= 0, "Do it right, that's not an option.");
	*/

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
