#include <stdio.h>
#include <stdint.h>
#include "dbg.h"

int main(int argc, char **argv)
{
	log_info("uintmax_t length: %ld", sizeof(uintmax_t));
	log_info("intptr_t length: %ld", sizeof(intptr_t));
	log_info("size_t length: %ld", sizeof(size_t));
	log_info("double length: %ld", sizeof(double));
	log_info("long double length: %ld", sizeof(long double));

	long register reg = 1000;
	log_info("register variable: %ld", reg);

	long l = 1;
	char c = '\0';
	int i = 1;
	double d = 1.0;

	log_info("size of expression: %ld", sizeof(l + c - i * d));
	log_info("size of expression: %ld", sizeof((int)l + c - (int)(i * d)));
	log_info("size of expression: %ld", sizeof((char)l + c - i * d));
	log_info("size of expression: %ld", sizeof((char)l + c - i * (char)d));
	log_info("size of expression: %ld", sizeof((char)l + c - (char)(i * d)));
	log_info("size of expression: %ld", sizeof((char)(i * d)));
	log_info("size of expression: %ld", sizeof((char)l));

	return 0;
}
