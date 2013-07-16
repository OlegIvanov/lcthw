#include <stdio.h>

// In this exercise I want you to attempt something difficult for the extra credit:
// Convert this program to not use pointers and malloc.

typedef struct {
	char *name;
	int age;
	int height;
	int weight;
} Person;

Person Person_create(char *name, int age, int height, int weight)
{
	Person who;

	who.name = name;
	who.age = age;
	who.height = height;
	who.weight = weight;

	return who;
}

void Person_print(Person who)
{
	printf("Name: %s\n", who.name);
	printf("\tAge: %d\n", who.age);
	printf("\tHeight: %d\n", who.height);
	printf("\tWeight: %d\n", who.weight);
}

int main (int argc, char *argv[])
{
	// make two people structures
	Person joe = Person_create("Joe Alex", 32, 64, 140);

	Person frank = {
		"Frank Blank", 20, 72, 180
	};

	Person_print(joe);
	Person_print(frank);

	// make everyone age 20 years and print them again
	joe.age += 20;
	joe.height -= 2;
	joe.weight += 40;
	Person_print(joe);

	frank.age += 20;
	frank.weight += 20;
	Person_print(frank);

	return 0;
}
