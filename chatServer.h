
void setConnection();
void *clientReadThread(void *args);
void sendMessage(int clientNum, char *buffer, char *name);
int getPortNumber(char *portNumber);
void checkArgs (int argc, char ** argv);