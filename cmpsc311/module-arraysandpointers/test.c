
// Library includes.
#include <stdio.h>

#include "extra.h"

// my_global_variable is DECLARED in extra.h, but not defined yet.
// Can arbitrarily define it here. Just so long as it is defined in the scope of the program?
int my_global_variable = 20;

int main() {

    

    print_global_var();
    
    return 0;
}
