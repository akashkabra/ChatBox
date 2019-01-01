

void checkArgs (int argc, char ** argv);
void startConnection(char *hostName);
int getPortNumber(char *portNumber);
void getIPfromHost(char *hostname);
void *threadRead(void *args);
void *threadSend(void *args);