#include "extentions.h"

#define COMMAND_COUNTER 38

int checkTheCommand(char (*commands)[12], char *command);
void handshakeFunc(int *sock, char *echoString, char* echoBuffer);
void openCommand(int *sock, char *ip, char *echoBuffer);
void disconnectFunc(int *sock, char *echoBuffer);
void consoleCommand();
void helpListCommand(char (*commands)[12]);
