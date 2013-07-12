#include <stdio.h>

int main(int argc, char *argv[])
{
	int distance = 100;
	float power = 2.345f;
	double super_power = 56789.4532;
	int* p_age = &distance;
	
	printf("Extra Credit 2.\n");
	printf("Percent sign: %%.\n");
	printf("Scientific notation (mantissa/exponent), uppercase (super_power): %E.\n", super_power);
	printf("Signed decimal integer (*p_age): %i.\n", *p_age);
	printf("Pointer address (p_age): %p.\n", p_age);
	
	printf("Extra Credit 3.\n");
	printf("Octal (distance): %o.\n", distance);
	printf("Hexadecimal (distance): %x.\n", distance);
	printf("Hexadecimal floating point, uppercase (power): %A.\n", power);

	printf("Extra Credit 4.\n");
	printf("Empty string: %s.\n", "");
	
	return 0;
}
