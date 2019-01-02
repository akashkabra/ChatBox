# ChatBox


This project is a chatting program between two clients which are connected to a server.

Note: This is a chat program between two users only, meaning only two clients are able to connect to the one server. Other pairs of clients will need to run the server on a different port.

Note: This program was developed under Linux Ubuntu (v18.04.1) using gcc compiler version 7.3.0

In this program, sockets and threads are used to make sure connections between clients and servers work properly.

Clients use different threads to send and read messages from the server.
Server uses threads to read everything the client says.


### Server
The server can be run from any computer by running the following command:
    ./chatServer <portNumber>
--> <portNumber> can be anything above 8000, but below 65535.
--> Please do not user port numbers between 0 and 1023 because these numbers are usually reserved.
--> A recommended range is: 10000 to 30000.

So, for example:    ./chatServer 12122

### Client
The client can be run from any computer by running the following command:
    ./chatClient <hostName> <portNumber>
    OR
    ./chatClient <ipAddress> <portNumber>
--> The hostname or ipAddress should be of where the server is running.
--> portNumber should be the same number as what the server has.

So, for example: ./chatClient localhost 12122
                 or
                ./chatClient 127.0.0.1 12122


The client will try to connect to the host every five seconds if connection failed, unless the input (hostname or port number) is wrong. 



Screenshots coming soon!