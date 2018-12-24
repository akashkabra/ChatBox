// includes ...


//To run the program after creating exec
//  ./chatServer <portNumber>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

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
        fprintf(STDERR, "Fatal Error: Wrong amount of arguments. Exiting.../n");
    }
}