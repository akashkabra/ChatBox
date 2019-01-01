#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#include "chatClient.h"

//To run the program after creating exec
//  ./chatClient <hostname> <portNumber>
//  or
//  ./chatClient <IP address> <portNumber>


char *G_ip;
int G_port = -1;
int IPorHost = -1;
int checkOrder = -1;    // 1 if hostname, 0 is IP address.

int main (int argc, char **argv) {

    //Setting up everything.
    checkArgs(argc, argv);
    G_port = getPortNumber(argv[2]);

    startConnection(argv[1]);

    printf(" testinggg12\n");
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

void startConnection(char * hostName) {
    //this is where all the socket and networking things will go.
    struct addrinfo hints;
    struct addrinfo *results;
    struct addrinfo *rp;
    int info = -1;
    int socketfd = -1;

    //finding the IP addresses
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    //Allows for IPv4 & IPv6
    hints.ai_socktype = SOCK_DGRAM;  //Datagram socket
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          // Any protocol

    printf("testing5\n");
    info = getaddrinfo(hostName, "12122", &hints, &results);
    if (info != 0) {
        fprintf(stderr, "Fatal Error: getaddrinfo: %s \n", gai_strerror(info));
        exit(-1);
    }
    /* 
    *   else .. getaddinfo() returns a list of IP addresses. Try each
    *   until one connects successful, or try again.
    */

   
   for(rp = results; rp != NULL; rp = rp->ai_next) {
       socketfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
       if (socketfd == -1) {
           printf("i wonder\n");
           continue;
       }
       if (connect(socketfd, rp->ai_addr, rp->ai_addrlen) != -1) {
           printf("success \n");
           break; //SUCCESS!
       }
       //close(socketfd);
   }
   if (rp == NULL) {
       //No addresses succeeded
       fprintf(stderr, "Fatal Error: Bad Address -- Count not connect.\n");
       exit(-1);
   }


   char *testMessage = "Client connected!";
   send(socketfd, testMessage, (strlen(testMessage)+1), 0);
   
   printf("test12\n");

}

void checkArgs(int argc, char **argv) {
    /*
    *   Make sure we have 3 arguments
    *   exec file name, Hostname or IP address, Port number
    *   And, check if input is IP address or hostname
    */
    if(argc != 3) {
        fprintf(stderr, "Fatal Error: Wrong amount of arguments. Exiting...\n");
        exit(-1);
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
            fprintf(stderr, "Fatal Error: %s is not a valid port number\n", portNumber);
            exit(-1);
        }
    }
    portNum = atoi(portNumber);
    return portNum;

}