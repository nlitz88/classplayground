#include <stdio.h>
#include <string.h>

int main() {

    // Here are some examples of string initialization.
    char *str1 = "abc";                         // This syntax accepts dynamic number of characters. Compiler will generate instrs accoridngly.
    // NOTE: I believe, despite being a pointer, this gets compiled by the compiler in the same way that an automatically would. (char [4])"abc".
    //       This is behavior specific to strings, I believe added as a convenience. This possible because the compiler can determine how much space to allocate at compile time.s
    char str2[] = "abc";                        // Compiler will allocate enough space for 4 characters, 'a', 'b', 'c', and one for the null terminator \0.
    char str3[4] = "abc";                       // This EXPLICITLY allocates enough space for "abc\0" (4 characters total).
    char str4[3] = "abcd"; // Wat?              // C will let you do this, but it will only allocate space for 3 characters. I.e., will store 'a','b','c'.
    // PROBLEM: No space allocate for NULL Terminator, and therefore it is not included. This is dangerous, as C won't know when to stop copying, for instance, in a strcpy!
    char str5[] = {'a', 'b', 'c', '\0'};        // This is fine, as the compiler will gen. instrs. to allocate enough space for all 4 characters, the \0 included!
    char str6[3] = {'a', 'b', 'c'};             // Again, no space allocated here for the null terminator. BAD.
    char str7[9] = {'a', 'b', 'c'};             // Because the chars of this string are EXPLICITLY initialized, a NULL terminator won't be inserted.

    // Copying Strings.
    char *myName = "Nathan";                // (char [7])"Nathan" Again, 7 elemens, the last ascii character is \0 == 0x00
    char nameBuf[20];
    // To copy string to another region of bytes (char array), can use strcpy.
    strcpy(nameBuf, myName);
    // This is great, however, if myName doesn't have a null terminator, then strcpy will just keep writing bytes beyond the end
    // of myName into nameBuf, overwriting the memory after nameBuf!
    // Here is an example:
    // Clear nameBuf.
    memset(nameBuf, ' ', 20);
    char myNameBad[6] = "Nathan";           // Observe, not enough space for null terminator to be added!
    // strcpy(nameBuf, myNameBad);             // Strcpy should just continue trying to copy. Why can't I observe with gdb?

    // To mitigate the risk of stepping over memory and overwriting something else, can use "n" variants of these string functions.
    // These let you specify the number of characters (bytes) you actually want to copy.
    strncpy(nameBuf, myName, 6);
    // NOTE: however, since myName doesn't have null terminator within the first 6 bytes, it won't be copied to the source either.
    //       Could manually add at the end if there is space, or could concatenate another string.

    // How can we add two strings together? CONCATENATE using strcat (or strncat).
    char lastName[] = " Litzinger";
    char fullName[25] = {'\0'};         // IMPORTANT: Because this is a string, we want to indicate that this is the beginning of the string!
    // Meaning that, when strcat operates on it, for instance, it will know that it can start copying bytes where that null terminator is. 
    // NOTE: The string that is being concatenated, its null terminator will also get tacked on, and act as the new null terminator for the whole string!
    strcat(fullName, myName);
    strcat(fullName, lastName);
    
    // How is strcat any different from strcpy?

    // Or, can use a safer strcat, that at most will only tack-on "n" bytes after the 


    return 0;
}