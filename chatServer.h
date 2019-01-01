

void checkArgs (int argc, char ** argv);
void setConnection();
int getPortNumber(char *portNumber);
void *acceptClientThread(void *args);
void *clientThread(void *args);