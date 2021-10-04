#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define RCVBUFSIZE 255 
#define PORT 1122

void dieWithError(char *str);
int checkIP(char *str);
void sendFunc(int *sock, char *echoString);
void sendIntFunc(int *sock, int num);
int receiveIntFunc(int *sock, int* num);
int receiveFunc(int *sock, char* echoBuffer);

