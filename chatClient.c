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
int G_Port = -1;
int IPorHost = -1;
int checkOrder = -1;    // 1 if hostname, 0 is IP address.


//All these are for IP address
struct addrinfo hints;
struct addrinfo *info;
struct addrinfo *infoTraverse;



int main (int argc, char **argv) {

    //Setting up everything.
    checkArgs(argc, argv);
    G_Port = getPortNumber(argv[2]);

    getIPfromHost(argv[1]);
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

void startConnection(char *hostname) {
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        fprintf(stderr, "Fatal Error: (socket()): %d: %s\n", errno, strerror(errno));
        exit(-1);
    }

    struct sockaddr_in serverAddr;  //Used for connect() in connection to server

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(G_Port);
    
    //Go through each IP address and try to connect to each IP until successfully connected.
    char ipAddress[256];
    for (infoTraverse = info; infoTraverse != NULL; infoTraverse = infoTraverse->ai_next) {
        getnameinfo(infoTraverse->ai_addr, infoTraverse->ai_addrlen, ipAddress, sizeof (ipAddress), NULL, 0, NI_NUMERICHOST);
        serverAddr.sin_addr.s_addr = inet_addr(ipAddress);
        puts(ipAddress);

        inet_pton(AF_INET, ipAddress, &serverAddr.sin_addr);

        int connectSuccess = -1;

        connectSuccess = connect(socketfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        printf("ashello?\n");
        printf("connectSuccess: %d \n", connectSuccess);
        if (connectSuccess == 0) {
            printf("Successfully Connected to host %s with IP %s\n", hostname, ipAddress);
            break;
        }
    }

    char *message = "Connection Successful!";
    send(socketfd, message, (strlen(message)+1), 0);

    
    

}

void getIPfromHost(char *hostname) {
    hints.ai_family = AF_INET;    //Allows for IPv4 only
    hints.ai_socktype = SOCK_DGRAM;  //Datagram socket
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          // Any protocol

    int result = getaddrinfo(hostname, NULL, &hints, &info);
    if(result != 0) {
        fprintf(stderr, "Fatal Error: getaddrinfo: %s \n", gai_strerror(info));
        exit(-1);
    }

/*  // make sure all the ip addresses print out.
    char host[256];
    for (infoTraverse = info; infoTraverse != NULL; infoTraverse = infoTraverse->ai_next) {

        getnameinfo(infoTraverse->ai_addr, infoTraverse->ai_addrlen, host, sizeof (host), NULL, 0, NI_NUMERICHOST);
        puts(host);
    }
*/
}




// void startConnection(char * hostName) {
//     //this is where all the socket and networking things will go.
//     struct addrinfo hints;
//     struct addrinfo *results;
//     struct addrinfo *rp;
//     int info = -1;
//     int socketfd = -1;

//     //finding the IP addresses
//     memset(&hints, 0, sizeof(struct addrinfo));
//     hints.ai_family = AF_INET;    //Allows for IPv4 only
//     hints.ai_socktype = SOCK_DGRAM;  //Datagram socket
//     hints.ai_flags = 0;
//     hints.ai_protocol = 0;          // Any protocol

//     printf("testing5\n");
//     info = getaddrinfo(hostName, "12122", &hints, &results);
//     if (info != 0) {
//         fprintf(stderr, "Fatal Error: getaddrinfo: %s \n", gai_strerror(info));
//         exit(-1);
//     }
//     /* 
//     *   else .. getaddinfo() returns a list of IP addresses. Try each
//     *   until one connects successful, or try again.
//     */

   
//     for(rp = results; rp != NULL; rp = rp->ai_next) {
//         socketfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
//         if (socketfd == -1) {
//             printf("i wonder\n");
//             continue;
//         }
//         if (connect(socketfd, rp->ai_addr, rp->ai_addrlen) != -1) {
//             printf("success \n");
//             break; //SUCCESS!
//         }
//         //close(socketfd);
//     }
//     if (rp == NULL) {
//         //No addresses succeeded
//         fprintf(stderr, "Fatal Error: Bad Address -- Count not connect.\n");
//         exit(-1);
//     }


//     char *testMessage = "Client connected!";
//     send(socketfd, testMessage, (strlen(testMessage)+1), 0);
    
//     char buffer[500];
//     int bufferLen = 500;
//     read(socketfd, buffer, bufferLen);
//     printf("%s\n", buffer);
   
//    printf("test12\n");

// }

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