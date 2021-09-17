#include <stdio.h>
#include <inttypes.h>
#include "extra.h"


void print_global_var() {
    // Can use my_global_variable, as a.) it is declared in the header file that is included in this file,
    // and b.) because it is defined at program level scope (somewhere else).
    printf("my_global_variable: %d\n", my_global_variable);
}


int showstack(int i, int j) {
    // Whenever this function called, i and j will both be pushed to the stack first.

    // Then, local variables defined here will be pushed to the stack as well.
    // i.e., k and the two elements of l.
    int k = 2;
    int32_t l[2];
    // %p used for pointer type data (printing addresses), lu = long unsigned integer.
    printf( "i is at [%p], size [%lu], value [%d]\n", &i, sizeof(i), i );
    printf( "j is at [%p], size [%lu], value [%d]\n", &j, sizeof(j), j );
    printf( "k is at [%p], size [%lu], value [%d]\n", &k, sizeof(k), k );
    printf( "l is at [%p], size [%lu], value [%d]\n", &l[1], sizeof(l[1]), l[1] );

    // Note how j actually gets pushed before i.

    return 0;
}