#include <stdio.h>

int main() {

    FILE *myFile = fopen("myData.txt", "r+");
    char testString[128];
    fgets(testString, 128, myFile);
    printf(testString);

    printf("\n");

    
    return 0;
}