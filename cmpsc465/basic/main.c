#include <stdint.h>
#include <stdio.h>

typedef uint8_t nameindex;

int main() {

    printf("Hello world! I'm %d years old!\n", 21);
    char *name = "Nathan";
    uint8_t name_size = 6;
    for (nameindex n = 0; n < name_size; ++n) {
        // Print the character and its address using pointer arithmetic.
        printf("\t%c at %p", name[n], ((char *)name + n));
    }
    printf("\n");
    printf("The array of bytes storing my name begins at %p\n", name);
    printf("The address of the variable storing the initial address of the array is %p\n", &name);
    return 0;
}