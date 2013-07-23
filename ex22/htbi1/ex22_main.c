#include "ex22.h"
#include "dbg.h"
#include <stdlib.h>

const char *MY_NAME = "Zed A. Shaw";

void scope_demo(int count)
{
	log_info("count is: %d", count);
	
	if(count > 10) {
		int count = 100; // BAD! BUGS!

		log_info("count in this scope is %d", count);
	}

	log_info("count is at exit: %d", count);

	count = 3000;

	log_info("count after assign: %d", count);
}

int main(int argc, char *argv[])
{
	// Add static or const specifiers to different variables and then try to change them.
	/*
	const int i = 10;
	log_info("i: %d", i);
	i = 100;
	*/

	// test out THE_AGE accessors
	log_info("My name: %s, age: %d", MY_NAME, get_age());

	set_age(100);

	log_info("My age is now: %d", get_age());

	// test out THE_SIZE extern
	log_info("THE_SIZE is: %d", THE_SIZE);
	print_size();

	THE_SIZE = 9;
	
	log_info("THE_SIZE is now: %d", THE_SIZE);
	print_size();
	
	// Try to directly access variables in ex22.c from ex22_main.c that you think you can't. 
	// For example, you can't get at ratio inside update_ratio? What if you had a pointer to it?
	// THE_AGE = 25;
	/*
	double **ratio_ptr_ptr = malloc(sizeof(void*));

	double *ratio_ptr = update_ratio(2.0, ratio_ptr_ptr);

	log_info("*ratio_ptr_ptr: %p", *ratio_ptr_ptr);
	log_info("**ratio_ptr_ptr: %f", **ratio_ptr_ptr);

	log_info("Ratio pointer value: %p", ratio_ptr);
	log_info("Ratio value: %f", *ratio_ptr);

	*ratio_ptr = 33;
	log_info("Ratio value: %f", *ratio_ptr);

	free(ratio_ptr_ptr);
	*/

	// test the ratio function static
	log_info("Ratio at first: %f", update_ratio(2.0));
	log_info("Ratio again: %f", update_ratio(10.0));
	log_info("Ratio once more: %f", update_ratio(300.0));

	// test the scope demo
	int count = 4;
	scope_demo(count);
	scope_demo(count * 20);

	log_info("count after calling scope_demo: %d", count);

	return 0;
}
