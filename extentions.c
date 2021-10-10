#include "extentions.h"

#define SERVER_CMD_COUNTER 28

char server_cmds[SERVER_CMD_COUNTER][12] = {"ABOR", "ACCT", "ACCT", "CD", "DELE", "HELP", "LIST", "MKD", "MODE", "NLST", "PASS", "PASV", "PORT", "PWD", "QUIT", "REIN", "RMD", "RNFR", "RNTO", "SITE", "SMNT", "STAT", "STOR", "STOU", "TYPE", "USER", "SYN", "ACK"};

void dieWithError(char *str) {
    printf("%s\n", str);
    exit(1);
}

int checkIP(char *str) {
    int octetCounter = 0;
    char *token, *last;
    char *tmp = strdup(str);
    char *ip = strtok(tmp, ":");

    token = strtok_r(tmp, ".", &last);
    while(token != NULL) {
        if(atoi(token) > 255)
            dieWithError("Invalid IP!");
        token = strtok_r(NULL, ".", &last);
        octetCounter++;
    }
    
    if(octetCounter == 4) 
        return 1;
    else
        return 0;
}

void sendFunc(int *sock, char *echoString) {
    unsigned int echoStringLen = strlen(echoString);

    if(send(*sock, echoString, echoStringLen, 0) != echoStringLen)
        dieWithError("send() sent a different number of bytes than expected");

    printf("Send: %s\n", echoString);
}

void sendIntFunc(int *sock, int num) {
    unsigned int numLen = sizeof(num);

    if(send(*sock, &num, numLen, 0) != numLen)
        dieWithError("send() sent a different number of bytes than expected");

    printf("\tSend code: %i\n\n", num);
}

int receiveIntFunc(int *sock, int *num) {
    int bytesRcvd = 0;

    if((bytesRcvd = recv(*sock, num, sizeof(num), 0)) <= 0)
        dieWithError("recv() failed or connection closed prematurely");

    printf("\tRecv code: %i\n\n", *num);

    return bytesRcvd;   
}

void sendResult(int *sock, int result) {
    if(!result) 
        sendFunc(sock, "226");                 // Успешно
    else
        sendFunc(sock, "451");                 // Операция прервана, ошибка
}

int receiveFunc(int *sock, char* echoBuffer) {
    int bytesRcvd = 0;

    if((bytesRcvd = recv(*sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
        dieWithError("recv() failed or connection closed prematurely");

    echoBuffer[bytesRcvd] = '\0';  
    printf("%s\n", echoBuffer);

    return bytesRcvd;   
}

/*
Враппер заменяет символ перехода на новую 
строку (\n) на символ конца строки (\0)
*/ 
char *fgets_wrapper(char *buffer, size_t buflen, FILE *fp) {
    if (fgets(buffer, buflen, fp) != 0) {
        buffer[strcspn(buffer, "\n")] = '\0';
        return buffer;
    }
    return 0;
}