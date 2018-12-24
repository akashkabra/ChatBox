// includes ...

//To run the program after creating exec
//  ./chatClient <hostname> <portNumber>
//  or
//  ./chatClient <portNumber> <hostname>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

char *G_ip;
int G_port = -1;

int main (int argc, char **argv) {

    //Setting up everything.
    checkArgs(argc, argv);
    G_ip = getIP(argv[1]);
    G_port = getPortNumber(argv[2]);
    
}

void checkArgs(int argc, char **argv) {
    /*
    *   Make sure we have 3 arguments
    *   exec file name, Hostname or IP address, Port number
    *   And, check if input is IP address or hostname
    */
    if(argc != 3) {
        fprintf(STDERR, "Fatal Error: Wrong amount of arguments. Exiting.../n");
    }
}

char *getIP(char *hostname) {

}

int getPortNumber(char *portNumber) {

}