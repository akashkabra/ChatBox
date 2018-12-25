#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "chatServer.h"

//To run the program after creating exec
//  ./chatServer <portNumber>


int main (int argc, char ** argv) {

    //Make sure arguments are right
    checkArgs(argc, argv);
}

void checkArgs(int argc, char ** argv) {
    /*
    *   Make sure we have 2 arguments
    *   1 - exec file name
    *   2 - Port number
    */
    if(argc != 2) {
        fprintf(stderr, "Fatal Error: Wrong amount of arguments. Exiting.../n");
    }
}