#include <lcthw/perfomance_routines.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/resource.h>

uint64_t get_diff(struct timespec start, struct timespec end)
{
	return (uint64_t)(end.tv_sec - start.tv_sec) * _BILLION_ + (uint64_t)(end.tv_nsec - start.tv_nsec);
}

void stack_increase()
{
	const rlim_t kStackSize = 256L * 1024L * 1024L;
	struct rlimit rl;
	int result;

	result = getrlimit(RLIMIT_STACK, &rl);
	if(result == 0) {
		if(rl.rlim_cur < kStackSize) {	
			rl.rlim_cur = kStackSize;

			result = setrlimit(RLIMIT_STACK, &rl);

            if(result != 0) {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
			}
		}
	}
}
