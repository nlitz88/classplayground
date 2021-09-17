#include <stdio.h>
#include "extra.h"


void print_global_var() {
    // Can use my_global_variable, as a.) it is declared in the header file that is included in this file,
    // and b.) because it is defined at program level scope (somewhere else).
    printf("my_global_variable: %d\n", my_global_variable);
}