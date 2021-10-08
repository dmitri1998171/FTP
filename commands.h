#include "extentions.h"

#define COMMAND_COUNTER 38

int checkTheCommand(char (*commands)[12], char *command);
int authLogin(int *sock, char* echoBuffer);
int authPasswrd(int *sock, char* echoBuffer);
int checkConnection(int *sock, char *ip, char *echoBuffer);
void openCommand(int *sock, char *ip, unsigned short echoServPort);
void disconnectFunc(int *sock, char *echoBuffer);
void helpListCommand(char (*commands)[12]);
void getFile(int *fileSock, char *filename);
void cdCommand(char *path);
void parseCommandLine(char *command, char **sndArg);
void CommandFunc(char *command, int *sock, int *fileSock, char *echoBuffer);
