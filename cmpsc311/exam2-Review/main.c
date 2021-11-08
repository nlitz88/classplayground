#include <stdio.h>
#include <string.h>

#include <helper.h>

int main() {
    
    // char myName[10];
    char *myName = "hello";
    printf("sizeof(myName): %d\n", (int)sizeof(myName));
    printf("strlen(myName): %d\n", (int)strlen(myName));

    // Scanf
    char firstName[20], lastName[20];
    int age;
    char sourceString[] = "Nathan Litzinger 21";
    sscanf(sourceString, "%s %s %d", firstName, lastName, &age);
    printf("%s %s %d\n", firstName, lastName, age);

    return 0;
}