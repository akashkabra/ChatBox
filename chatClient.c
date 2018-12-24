#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

//To run the program after creating exec
//  ./chatClient <hostname> <portNumber>
//  or
//  ./chatClient <IP address> <portNumber>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

char *G_ip;
int G_port = -1;
int IPorHost = -1;
int checkOrder = -1;    // 1 if hostname, 0 is IP address.

int main (int argc, char **argv) {

    //Setting up everything.
    checkArgs(argc, argv);
    G_port = getPortNumber(argv[2]);

    startConnection();

/*
    if(checkOrder == 1) {
        //need to convert hostname to IP address
        G_ip = getIPfromHost(argv[1]);
    }
    if(checkOrder == 0) {
        //Don't need to do anything.. already an IP Address.
    }
*/


    
}

void startConnection() {
    //this is where all the socket and networking things will go.
    
}

void checkArgs(int argc, char **argv) {
    /*
    *   Make sure we have 3 arguments
    *   exec file name, Hostname or IP address, Port number
    *   And, check if input is IP address or hostname
    */
    if(argc != 3) {
        fprintf(STDERR, "Fatal Error: Wrong amount of arguments. Exiting.../n");
        exit(0);
    }

    int checkFirstArg = isalpha(argv[1][0]);
    if (checkFirstArg > 0) {
        printf("This is a letter.\n");
        checkOrder = 1;
    } else {
        printf("This is NOT a letter.\n");
        checkOrder = 0;
    }
}

int getPortNumber(char *portNumber) {
    int portNum = 0;
    int i = 0;
    for (i = 0; i < strlen(portNumber); i++) {
        if(!isdigit(portNumber[i])) {
            fprintf(STDERR, "Fatal Error: %s is not a valid port number\n", portNumber);
            exit(0);
        }
    }
    portNum = atoi(portNumber);
    return portNum;

}