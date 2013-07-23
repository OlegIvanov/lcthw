#include <stdio.h>
#include "dbg.h"

void pass_by_value(int param)
{
	log_info("pass_by_value: param = %d", param);
	param = 100;
}

void pass_by_reference(int* param_ref)
{
	log_info("pass_by_reference: param = %d", *param_ref);
	*param_ref = 100;
}

int main(int argc, char **argv)
{
	int param = 10;

	pass_by_value(param);
	log_info("main: param = %d", param);

	pass_by_reference(&param);
	log_info("main: param = %d", param);

	return 0;
}
