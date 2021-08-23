#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define COMMAND_COUNTER 38

int checkTheCommand(char (*commands)[12], char *command);
void openCommand(char *ip);
void consoleCommand();
void helpListCommand(char (*commands)[12]);
int sendFunc(int *sock, char *echoString);
void receiveFunc(int *sock, char* echobuffer, unsigned int echoStringLen);
