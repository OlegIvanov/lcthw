// The code in the book up to this point (and for the rest of it) potentially violates these rules. 
// Go back through and apply what you've learned to one exercise to see if you can improve it or find bugs.

// Use scanf to write a function that reads a character at a time and files in the names but doesn't go past the end. 
// Make this function generic so it can take a size for the string, and make sure you end the string with '\0' no matter what.

#undef NDEBUG
#include <stdio.h>
#include "dbg.h"
#include <assert.h>

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

int read_name(char *name, int size)
{
	assert(name != NULL && "Parameter 'name' can't be NULL.");
	check(size > 0, "Parameter 'size' should be greater than 0.");

	int i = 0;
	char c;
	
	for(i = 0, scanf("%c", &c); 
		i < size - 1 && c != '\n';
		i++, scanf("%c", &c)) {

		*(name + i) = c;
	}
	
	*(name + i) = '\0';
	
	// skipping the rest of the line
	if(c != '\n') {
		scanf("%*[^\n]");
		scanf ("%*c");
	}

	return 0;

error:
	return -1;
}

int main(int argc, char *argv[])
{
	Person you = {.age = 0};
	int i = 0;
	int rc = 0;

	printf("What's your First Name? ");
	read_name(you.first_name, MAX_DATA);
	check(rc == 0, "Failed to read first name.");

	printf("What's your Last Name? ");
	read_name(you.last_name, MAX_DATA);
	check(rc == 0, "Failed to read last name.");

	printf("How old are you? ");
	rc = fscanf(stdin, "%d", &you.age);
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
	
	printf("First Name: %s\n", you.first_name);
	printf("Last Name: %s\n", you.last_name);
	printf("Age: %d\n", you.age);
	printf("Eyes: %s\n", EYE_COLOR_NAMES[you.eyes]);
	printf("Income: %f\n", you.income);
	
	return 0;
error:

	return -1;
}
