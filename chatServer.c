#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h> 

#include "chatServer.h"

//To run the program after creating exec
//  ./chatServer <portNumber>

int G_port = -1;
int G_socketfd = -1;    
struct sockaddr_in G_address;   //Address
int G_addrlen = sizeof(G_address);  //size of address --> accept()

int clientList[2];

// Need a mutex for printing out when clients disconnect
pthread_mutex_t mutex_Closing = PTHREAD_MUTEX_INITIALIZER;


int main (int argc, char ** argv) {

    //Make sure arguments are right
    checkArgs(argc, argv);
    G_port = getPortNumber(argv[1]);

    //Set up a signal to catch ctrl + c, which ends program
    signal(SIGINT, sigHandler);

    setConnection();

    return 0;
}

//Find clients trying to connect
void setConnection() {

    G_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (G_socketfd == -1) {
        fprintf(stderr, "Fatal Error: (socket() error) %d: %s \n", errno, strerror(errno));
        exit(-1);
    }

    int opt = 1;
    if (setsockopt(G_socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        fprintf(stderr, "Fatal Error: (setsockopt() error) %d: %s\n", errno, strerror(errno));
        exit(-1);
    }

    G_address.sin_family = AF_INET; 
    G_address.sin_addr.s_addr = INADDR_ANY;   // Any address
    G_address.sin_port = htons(G_port);       // Port number

    // Forcefully attach socket to port
    if (bind(G_socketfd, (struct sockaddr *)&G_address, sizeof(G_address)) != 0) {
        fprintf(stderr, "Fatal Error: (bind() error) %d: %s\n", errno, strerror(errno));
        exit(-1);
    }

    int clientCounter = 0;
    //Only two clients can connect, if more try to connect, they get an error.
    //This will also make sure program keeps running.
    while(1) {
        listen(G_socketfd, 2);
        int tempAccept = accept(G_socketfd, (struct sockaddr *)&G_address, (socklen_t *)&G_addrlen);
        //send(tempAccept, "Hello", 6, 0);
        //printf("hello??\n");
        if (tempAccept < 0) {
            fprintf(stderr, "Fatal Error: (accept() error) %d: %s\n", errno, strerror(errno));
            exit(-1);
        }
        //printf("SUCCESS \n");
        // Make sure only two client can connect.
        if (clientCounter == 2) {
            char *failMessage = "Two Clients already connected";
            send(tempAccept, failMessage, (strlen(failMessage) +1), 0);
            continue;
        }
        clientList[clientCounter] = tempAccept;
        char *ackConnection = "Connection Successful!";
        send(tempAccept, ackConnection, (strlen(ackConnection) +1), 0);

        //thread for the server to read messages from the client
        pthread_t tidRead;
        pthread_create(&tidRead, NULL, clientReadThread, &tempAccept);
        clientCounter++;
    }
}

//This is where everything regarding the client occurs.
void *clientReadThread(void *args) {
    int *temp = (int*)args;
    int clientNum = *temp;
    printf("clientNum: %d connected!\n", clientNum);

    char name[50];
    char finalName[50];
    read(clientNum, name, 50);
//    printf("Name: %s", name);
    
    for(int i = 0; i < strlen(name)+1; i++) {
        if (name[i] == '\n') {
            break;
        }
        finalName[i] = name[i];
    }
    
    char *connectedTo = NULL;
    
    connectedTo = (char*)malloc(sizeof(char) * (strlen(name) + 14));
    strcat(connectedTo, "Connected to ");
    strcat(connectedTo, name);
    strcat(connectedTo, "\n");

    if(clientList[0] != clientNum) {
        send(clientList[0], connectedTo, (strlen(connectedTo) + 1), 0);
    } else if (clientList[1] != clientNum) {
        send(clientList[1], connectedTo, (strlen(connectedTo) + 1), 0);
    }

    char buffer[500];
    int bufferLen = 500;
    //Read from both clients at any given time.
    while (1) {
        read(clientNum, buffer, bufferLen);
        if(strcmp(buffer, "Client Closing Down") == 0) {
            pthread_mutex_lock(&mutex_Closing);
            printf("Client number %d disconnected.\n", clientNum);
            //client is closing.
            if(clientList[0] != clientNum) {
                send(clientList[0], "Other user disconnected.", 25, 0);
                printf("Client number %d disconnected.\n", clientList[0]);
            } else if (clientList[1] != clientNum) {
                send(clientList[1], "Other user disconnected.", 25, 0);
                printf("Client number %d disconnected.\n", clientList[1]);
            }
            continue;
            pthread_mutex_unlock(&mutex_Closing);
        }
        sendMessage(clientNum, buffer, finalName);
//        printf("%s", buffer);
    }
}

//Send the message from one client to another client and vice versa
void sendMessage(int clientNum, char *buffer, char *name) {
    char *finalStr = NULL;
    finalStr = (char*)malloc(sizeof(char) * ((strlen(buffer) + strlen(name))));
    strcat(finalStr, name);
    strcat(finalStr, ": ");
    strcat(finalStr, buffer);

    //The message sent also shows the client's name.
    if(clientList[0] != clientNum) {
        send(clientList[0], finalStr, (strlen(finalStr) + 1), 0);
    } else if (clientList[1] != clientNum) {
        send(clientList[1], finalStr, (strlen(finalStr) + 1), 0);
    }
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

void checkArgs(int argc, char ** argv) {
    /*
    *   Make sure we have 2 arguments
    *   exec file name, port number
    */
    if(argc != 2) {
        fprintf(stderr, "Fatal Error: Wrong amount of arguments. Exiting.../n");
        exit(-1);
    }
}

void sigHandler (int sigNum) {
    printf("Ctrl+C was pressed. Server shutting down...\n");
    exit(-1);
}