#ifndef EX22_H_
#define EX22_H_

// make THE_SIZE in ex22.c available to other .c files
extern int THE_SIZE;

// gets and sets an internal static variablein ex22.c
int get_age();
void set_age(int age);

// updates a static variable that's inside update_ratio
double update_ratio(double ratio);

void print_size();

#endif
