#ifndef EXTRA_H
#define EXTRA_H

// This gets included when the header file included. Could just redeclare it every where you use it,
// just not necessary in this case, as this header file is included wherever it's used. Would need to
// be otherwise.
extern int my_global_variable;

// Function to print my_global_variable.
void print_global_var();

// Function to show stack.
int showstack(int i, int j);

#endif