
// Library includes.
#include <stdio.h>
#include <inttypes.h>

#include "extra.h"

// my_global_variable is DECLARED in extra.h, but not defined yet.
// Can arbitrarily define it here. Just so long as it is defined in the scope of the program?
int my_global_variable = 20;

int main() {

    print_global_var();

    // Statically sized array.
    // 10 32 bit integers will get pushed to the stack.
    // The sp will just be decremeted, but no values will be initialized.
    int32_t numbers[10];
    printf("Elements of array 1 are %lu bits\n", sizeof(numbers[1])*8);

    // 10 <WORD_SIZE of OS> will get pushed to the stack. //ACTUALLY NO, DEPENDS ON THE COMPILER at this point.
    // In this case, the values of the first 4 elements will be initialized to 1.
    // The rest should be indeterminate, or zero, depending on the compiler.
    int numbers2[10] = {[0 ... 3] = 1};
    // The size must be a constant, as you can't change the amount that the stack pointer is
    // incremented again once done with that function/context.
    printf("Elements of array 2 are %lu bits\n", sizeof(numbers2[1])*8);


    const int ROWS = 2; // could be used in place of the "10" literal.
    const int COLS = 20;

    // Statically allocated to stack 2D array!
    // Must either initialize completely or not at all!
    // int matrix[ROWS][COLS] = {{1,2}, {1,1}};    // invalid!
    int matrix[ROWS][COLS];     //valid

    // function that will print out the addresses of elements on the stack upon the function call.
    // Note how, because these values are passed by value, the addresses of these arguments printed
    // will be the addresses of new locations on the stack containing these arguments' values.
    showstack(3, ROWS);
    
    return 0;
}
