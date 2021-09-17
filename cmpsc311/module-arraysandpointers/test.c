
// Library includes.
#include <stdio.h>
#include <inttypes.h>
#include <time.h>

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


    // Iterating through an array using pointers.
    int ages[4] = {10, 12, 20, 19};

    // Iterating through using integer as iterator.
    int i;
    __clock_t begin = clock();
    for(i = 0; i < 4; ++i) {
        // IN C, address arithmetic treats literals as word address increments. In other words, saying "address + 1" is really
        // incrementing the byte address of that variable by 1x4 addresses.
        // printf("elem %d: %d\n", i, *(ages + i));
        printf("Addr of ages[%d]: %p\n", i, &ages[i]);
        // OBSERVE: Though the stack grows downwards (I.e., new elems have lower addresses),
        //          array elems get pushed in the opposite order. In other words, the last elem gets pushed
        //          first. This is why when we INCREMENT the base adddress of our array, so that we can
        //          move UP the stack to the "later" elements of the array.
    }
    __clock_t end = clock();
    double timeSpent1 = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("That for loop took %f seconds to complete\n", timeSpent1);

    // Can also step through an array using pointers like this (more efficient).
    // The basic idea is to keep incrementing a pointer that starts at the array's base address
    // until that pointer is at the address after the last element.

    // First, need a pointer to keep track of the end of the array so that it doesn't need to be calculated
    // on every iteration.
    int *oneAfterLast = ages + 4;
    // Could also define as = &ages[4], both get the address right after the address of the last element/
    // Then, Make a pointer that is incremented as we traverse through the stack addresses.
    // This pointer starts at the address of ages.
    // The condition cursor != oneAfterLast says that we can reference each item in the array, and once our cursor
    // goes to an address after our last element, we can't go any further.
    // Could also do something like "lastElemsAddr - Cursor >= 0", but then this introduces a more involved branch
    // decision for each iteration == slower.
    begin = clock();
    for(int *cursor = ages; cursor != oneAfterLast; cursor += 1) {
        
        printf("Addr of Ages: %p\n", cursor);

    }
    end = clock();
    double timeSpent2 = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("That for loop took %f seconds to complete\n", timeSpent2);
    printf("Loop 2 was %f times faster than Loop 1\n", (double)(timeSpent1/timeSpent2));
    
    return 0;
}
