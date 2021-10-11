#include <stdio.h>
#include <stdint.h>
#include <string.h> // memcpy, memcmp
#include <stdlib.h> // malloc, calloc


// Simple function that takes in a number of bytes it needs space for.
// Function will call malloc to allocate this memory in the heap, and
// then return the starting address of this region.
char *readFile(int byteCount) {
    char *buf = malloc(byteCount * sizeof(char));
    // Could also use calloc here.
    // char *buf = calloc(byteCount, sizeof(char));

    // Also, ALWAYS do a null check in case dynamic allocation fails.
    if(buf == NULL) {
        // Would rather assign a NULL and then run into a segmentation fault than corrupting memory
        // unintentionally!
        return NULL;
    }
    return buf;
}


// Sample function to demonstrate perhaps more performant way of dynamically allocating memory using a memory map!
int mymap( char val ) {
    // Local variables
    int fh, i;
    char *ptr;
    // Open the file and "map" 20 bytes of it to memory
    // if ( (fh = open( "mmap.dat", O_CREAT|O_RDWR )) == 0 ) {
    //     printf( "Error in open [%s], aborting\n", strerror(errno) );
    //     return( -1 );
    // }
    // if ( (ptr = mmap( NULL, 20, PROT_READ|PROT_WRITE, MAP_SHARED, fh, 0 )) == (void *)-1 ) {
    //     printf( "Error in map [%s], aborting\n", strerror(errno) );
    //     return( -1 );
    // }
    // Add the values
    for (i=0; i<20; i++) {
        ptr[i] = val; // Replicate the value into the file
    }
    // "free" the mapped memory, close the file
    munmap( ptr, 20 );
    close( fh );
    return( 0 );
}


int pointerTesting() {
    char hi[6] = {'h','e','l','l','o','\0'};char*p, **dp;p= &(hi[0]);dp= &p;printf("%c %c\n", *p, **dp);printf("%p %p %p\n",p, *dp,hi);p+=1;printf("%c %c\n", *p, **dp);printf("%p %p %p\n",p, *dp,hi);*dp+=2;printf("%c %c\n", *p, **dp);printf("%p %p %p\n",p, *dp,hi);return 0;
}


int main() {

    // Here is an example of pointer type casting.
    int intArray[4] = {1, 2, 3, 4};
    // Void buffer. Means that until it is cast, the compiler has no idea what kind of arithmetic
    // rules to follow/what kind of instructions to generate to work with pointers of this type.
    // Therefore, if not cast, depending to the compiler, it'll treat it as the most primitive, lowest level
    // (byte addressable).
    void* buf;
    buf = &intArray[0];     // buf maintains the address of the first element in intArray. Same as = intArray.

    // Then, based on the type that the pointer is cast to, see what kind of data is printed out.
    printf("buf[1] when buf points to an int: %d\n", *((int *)buf + 1));
    printf("buf[1] when buf points to a char: %d\n", *((char *)buf + 4));
    // The second print statement will actually print out the first byte value of the int at buf + 1!
    // Incrementing a byte address by 4 is the same as incrementing an integer (word) address by 1.
    // The compiler will generate instructions to only increment the address by 1 if a char address.
    
    // Here, the compiler just treats the uncasted void pointer at the lowest level.
    // printf("%p", buf + 1);

    // Working with buffers.
    uint16_t buf1[10] = {0,1,2,3,4,5,6,7,8,9};
    uint16_t buf2[20] = {[0 ... 19] = 0};

    // Here's an example of using memcpy to copy all 10 elements of buf1 to the last 10 spaces in buf2.
    memcpy((buf2 + 10), buf1, 10*sizeof(uint16_t));
    // memcpy((buf2 + 10), buf1, 10);       THIS WOULDN'T WORK, as we're specifying the number of BYTES.
    // All of the c-type functions like this (malloc, realloc, memcpy, memcmp) use bytes!
    printf("20th element of buf2, where buf1 started copying to: %d\n", buf2[19]);

    // Here memcmp (memory compare) is demonstrated. This compares n bytes of one buffer/location
    // starting at buf1 and starting at buf2 (addresses).
    int result = memcmp(buf1, &buf2[10], 10*sizeof(uint16_t));
    printf("Result of comparison: %d\n", result);
    // 0 if all bytes equal, positive if second buffer provided's value > first. Negative if first larger.


    // Dynamic memory allocation example.
    //  Defined a function that will allocate enough memory for a particular file.
    char* myBuf = readFile(10);
    // Then, once done with memory at myBuf, can free it.
    free(myBuf);
    // When allocated, malloc allocates slightly more space to store the metadata of that allocation
    // like its size and maybe other properties. Thus, free knows how many bytes it can release or
    // make "up for grabs" when called.

    // We'd rather cause a segmentation fault than corrupting memory unknowingly. When a pointer
    // with NULL as the address is attempted to be dereferenced, a segmentation fault is caused,
    // which throws an exception and execution stops. This is prefereable over causing unpredictable
    // program behavior.
    myBuf = NULL;

    pointerTesting();

    return 0;
}


