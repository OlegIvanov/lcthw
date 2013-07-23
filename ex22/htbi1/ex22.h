#ifndef _ex22_h
#define _ex22_h

// makes THE SIZE in ex22.c available to other .c files
// Ditch the extern declaration in ex22.h to see what you get for errors or warnings.
extern int THE_SIZE;

// gets and sets an internal static variable in ex22.c
int get_age();
void set_age(int age);

/*
double *update_ratio(double ratio, double **ratio_ptr);
*/

// updates a static variable that's inside update_ratio
double update_ratio(double ratio);

void print_size();

#endif
