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

#include "chatServer.h"

//To run the program after creating exec
//  ./chatServer <portNumber>

int G_port = -1;
int G_socketfd = -1;    
struct sockaddr_in G_address;   //Address
int G_addrlen = sizeof(G_address);  //size of address --> accept()

//Mutex for changing clientCounter in acceptClientThread() --> USE FOR LATER
pthread_mutex_t mutex_G_clientCounter = PTHREAD_MUTEX_INITIALIZER;
int G_clientCounter = 0;  //Max of 2 clients


int main (int argc, char ** argv) {

    //Make sure arguments are right
    checkArgs(argc, argv);
    G_port = getPortNumber(argv[1]);

    setConnection();
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


    // NOW.... when listening and accepting --> Create a new thread and put listening and accept in there.
    // And then.. send the return value of accept() to another thread to do all the work for that client.

    pthread_t tid;
    pthread_create(&tid, NULL, acceptClientThread, NULL);

    //make sure program doens't end
    while(1) { }
}


//To listen and accept multiple clients
void *acceptClientThread(void *args) {
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

        pthread_t tid;
        pthread_create(&tid, NULL, clientThread, &tempAccept);
    }
}

//This is where everything regarding the client occurs.
void *clientThread(void *args) {
    int *temp = (int*)args;
    int clientNum = *temp;
    printf("clientNum: %d \n", clientNum);

    char buffer[500];
    int bufferLen = 500;
    while (1) {
        read(clientNum, buffer, bufferLen);
        printf("%s", buffer);
    }

}
