

void checkArgs (int argc, char ** argv);
void setConnection();
int getPortNumber(char *portNumber);
void *listenAndAcceptThread(void *args);
void *clientThread(void *args);