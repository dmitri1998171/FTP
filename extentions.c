#include "extentions.h"

#define SERVER_CMD_COUNTER 26

char server_cmds[SERVER_CMD_COUNTER][12] = {"ABOR", "ACCT", "ACCT", "CWD", "DELE", "HELP", "LIST", "MKD", "MODE", "NLST", "PASS", "PASV", "PORT", "PWD", "QUIT", "REIN", "RMD", "RNFR", "RNTO", "SITE", "SMNT", "STAT", "STOR", "STOU", "TYPE", "USER"};

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

int sendFunc(int *sock, char *echoString) {
    int echoStringLen = strlen(echoString);

    if(send(*sock, echoString, echoStringLen, 0) != echoStringLen)
        dieWithError("send() sent a different number of bytes than expected");

    return echoStringLen;
}

void receiveFunc(int *sock, char* echoBuffer, unsigned int echoStringLen) {
    int bytesRcvd = 0, totalBytesRcvd = 0;   

    printf("Received: ");
    while (totalBytesRcvd < echoStringLen) {
        if((bytesRcvd = recv(*sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
            dieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd;   
        echoBuffer[bytesRcvd] = '\0';  
        printf("%s", echoBuffer);      
    }

    printf("\n");
}
