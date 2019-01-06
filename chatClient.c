#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#include "chatClient.h"

//To run the program after creating exec
//  ./chatClient <hostname> <portNumber>
//  or
//  ./chatClient <IP address> <portNumber>

//Messages sent/received can be 255 - (length of name) characters long

int G_Port = -1;
int G_socketfd = -1;

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
}

// Sets up the whole socket and connection with server.
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
    char ipAddress[64];
    int finished = 1;
    while(finished == 1) {
        for (infoTraverse = info; infoTraverse != NULL; infoTraverse = infoTraverse->ai_next) {
            getnameinfo(infoTraverse->ai_addr, infoTraverse->ai_addrlen, ipAddress, sizeof (ipAddress), NULL, 0, NI_NUMERICHOST);
            serverAddr.sin_addr.s_addr = inet_addr(ipAddress);
            //puts(ipAddress);

            inet_pton(AF_INET, ipAddress, &serverAddr.sin_addr);

            int connectSuccess = -1;
            connectSuccess = connect(socketfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            //printf("connectSuccess: %d\n", connectSuccess);
            if (connectSuccess == 0) {
                // Make sure connection is set.
                char buffer[50];
                read(socketfd, buffer, 50);
                if(strcmp(buffer, "Server Message: Connection Successful!") != 0) {
                    fprintf(stderr, "Fatal Error: Failed to connect. (Two clients already connected.)\n");
                    exit(-1);
                }
                printf("Successfully Connected to host \"%s\" with IP %s\n", hostname, ipAddress);
                finished = 0;
                break;
            } else {
                fprintf(stderr, "Error: Connection to host \"%s\" with IP %s failed\n", hostname, ipAddress);
            }
        }
        if(finished == 1) {
            printf("Trying again in 5 seconds...\n");
            sleep(5);
        }
    }
    //remove socketfd, and replace it with G_socketfd later. 
    //For now, this is only for the signal, which was added near the end. 
    G_socketfd = socketfd;

    // Setting up signal once client is connected to the server.
    // This will catch ctrl+c, which ends the program
    signal(SIGINT, sigHandler);

    //Client enters their name or nickname for other client to see
    char name[50];
    printf("Please enter your first name: ");
    fgets(name, 50, stdin);
    send(socketfd, name, 50, 0);

    // Threads for the client to send and read messages at any time.
    pthread_t tidRead;
    pthread_t tidSend;
    pthread_create(&tidRead, NULL, threadRead, &socketfd);
    pthread_create(&tidSend, NULL, threadSend, &socketfd);

    while(1) { /* Keep programing running */}


}

// Thread for getting messages from the server (from the other client)
void *threadRead(void *args) {
    int *temp = (int*)args;
    int serverID = *temp;
    //Messages can be 255 characters long -> (255 - length of name)
    char buffer[256];
    int length = 256;

    while(1) {
        strcpy(buffer, "somethingrandomthatnooneWillType");
        read(serverID, buffer, length);
        if(strcmp(buffer, "Server Message: Other user disconnected.") == 0) {
            //printf("Server: %s", buffer);
            printf("%s. ", buffer);
            printf("Ending program now.\n");
            exit(-1);
        }
        if(strcmp(buffer, "Server Message: Server shutdown.") == 0) {
            printf("%s\n", buffer);
            printf("Shutting down program.\n");
            exit(-1);
        }
        if(strcmp(buffer, "somethingrandomthatnooneWillType") != 0) {
            //printf("Server: %s", buffer);
            printf("%s", buffer);
        }
    }
}

// Thread for sending messages to the server (which sends to other client)
void *threadSend(void *args) {
    int *temp = (int*)args;
    int serverID = *temp;
    //Messages can be 255 characters long -> (255 - length of other client name)
    char buffer[256];
    int length = 256;

    while(1) {
        fgets(buffer, length, stdin);
        send(serverID, buffer, length, 0);
    }
}


void getIPfromHost(char *hostname) {
    hints.ai_family = AF_INET;    //Allows for IPv4 only
    hints.ai_socktype = SOCK_DGRAM;  //Datagram socket
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          // Any protocol

    int result = getaddrinfo(hostname, NULL, &hints, &info);
    if(result != 0) {
        fprintf(stderr, "Fatal Error: getaddrinfo: %s\n", gai_strerror(result));
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

//Make sure the entered port number is actually a number.
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
}

void sigHandler (int sigNum) {
    printf("Ctrl+C was pressed. Client shutting down...\n");
    send(G_socketfd, "Client Closing Down", 20, 0);
    exit(-1);
}