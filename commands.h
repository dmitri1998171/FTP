#include "extentions.h"

#define COMMAND_COUNTER 38

int checkTheCommand(char (*commands)[12], char *command);
void handshakeFunc(int *sock, char *echoString, char* echoBuffer);
void authLogin(int *sock, char* echoBuffer);
void authPasswrd(int *sock, char* echoBuffer);
void checkConnection(int *sock, char *ip, char *echoBuffer);
void openCommand(int *sock, char *ip, unsigned short echoServPort);
void disconnectFunc(int *sock, char *echoBuffer);
void consoleCommand();
void helpListCommand(char (*commands)[12]);
void lsCommand(int *sock, char *echoBuffer);
