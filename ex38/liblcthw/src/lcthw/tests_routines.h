#ifndef _perfomance_routines_h
#define _perfomance_routines_h

#include <stdint.h>
#include <time.h>

#define _BILLION_ 1000000000UL

uint64_t get_diff(struct timespec start, struct timespec end);

void stack_increase();

#endif
