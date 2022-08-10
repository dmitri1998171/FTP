#include "extentions.h"

#define COMMAND_COUNTER 38

int checkTheCommand(char (*commands)[12], char *command);
int authLoginPasswrd(int *sock, char* echoBuffer);
int authPasswrd(int *sock, char* echoBuffer);
int checkConnection(int *sock, char *ip, char *echoBuffer);
void ValidateConnection(int *sock, int *fileSock, char *ip, char *echoBuffer, int *authChecker, int *dataChecker);
void openCommand(int *sock, char *ip, unsigned short echoServPort);
void disconnectFunc(int *sock, int *fileSock, char *echoBuffer);
void helpListCommand(char (*commands)[12]);
void getFile(int *fileSock, char *filename);
void cdCommand(int *sock, char *echoBuffer, char *path);
void mkdirCommand(int *sock, char *echoBuffer, char *path);
void rmdirCommand(int *sock, char *echoBuffer, char *path);
void CommandFunc(char *command, int *sock, int *fileSock, char *echoBuffer);
int getFunc(int *sock, int *fileSock, char *echoBuffer, char* filename);
