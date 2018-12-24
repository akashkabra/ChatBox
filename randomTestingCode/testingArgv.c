#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>


int main (int argc, char ** argv) {

    printf("%c\n", argv[1][0]);

    char a = 'g';
    char b = '1';

    if (isalpha(a) > 0) {
        printf("this is a letter\n");
    } else {
        printf("This is not a letter\n");
    }

    if (isalpha(b) > 0) {
        printf("this is a letter\n");
    } else {
        printf("This is not a letter\n");
    }
}
