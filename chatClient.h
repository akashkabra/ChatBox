
void startConnection(char *hostName);
void *threadRead(void *args);
void *threadSend(void *args);
void getIPfromHost(char *hostname);
int getPortNumber(char *portNumber);
void checkArgs (int argc, char ** argv);
void sigHandler (int sigNum);