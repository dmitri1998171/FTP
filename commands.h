#include "extentions.h"

#define COMMAND_COUNTER 38

int checkTheCommand(char (*commands)[12], char *command);
void handshakeFunc(int *sock, char *echoString, char* echoBuffer);
void openCommand(char *ip);
void consoleCommand();
void helpListCommand(char (*commands)[12]);
