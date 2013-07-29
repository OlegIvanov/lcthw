#include <stdio.h>
#include <string.h>
#include "dbg.h"
#include <dlfcn.h>

typedef int (*lib_function)(const char *data);
typedef int (*change_case_function)(const char *data, int size);

int main(int argc, char *argv[])
{
	int rc = 0;
	check(argc == 4, "USAGE: ex29 libex29.so function data");

	char *lib_file = argv[1];
	char *func_to_run = argv[2];
	char *data = argv[3];

	void *lib = dlopen(lib_file, RTLD_NOW);
	check(lib != NULL, "Failed to open the library %s: %s", lib_file, dlerror());

	void *func = dlsym(lib, func_to_run);
	check(func != NULL, "Did not find %s function in the library %s: %s", func_to_run, lib_file, dlerror());

	if(strcmp("uppercase", func_to_run) == 0 || strcmp("lowercase", func_to_run) == 0) {
		rc = ((change_case_function)func)(data, strlen(data) + 1);
	} else {
		rc = ((lib_function)func)(data);
	}
	check(rc == 0, "Function %s return %d for data: %s", func_to_run, rc, data);

	rc = dlclose(lib);
	check(rc == 0, "Failed to close %s", lib_file);

	return 0;

error:
	return 1;
}
