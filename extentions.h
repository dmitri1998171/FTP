#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define RCVBUFSIZE 255 
#define PORT 1122

void dieWithError(char *str);
int checkIP(char *str);
void sendFunc(int *sock, char *echoString);
int receiveFunc(int *sock, char* echoBuffer);
void sendIntFunc(int *sock, int num);
int receiveIntFunc(int *sock, int* num);
void sendResult(int *sock, int result);
void recvResult(int *sock,  int *fileSock, char* echoBuffer, char* localBuffer);
char *fgets_wrapper(char *buffer, size_t buflen, FILE *fp);
int readFile(char *filename, char *buffer);
void writeFile(char *filename, char *buffer); 
void sendFileFunc(int *sock, char *filename);
void receiveFile(int *sock, char *filename);
