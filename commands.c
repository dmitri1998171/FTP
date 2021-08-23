#include "extentions.h"

int checkTheCommand(char (*commands)[12], char *command) {
    int command_checker = 0;
    
    for(int i = 0; i < COMMAND_COUNTER; i++) {
        if(!strcmp(commands[i], command)) {
            command_checker = 1;
            break;
        }
    }

    return command_checker;
}

void openCommand(char *ip) {
    unsigned short echoServPort = 1026;
    unsigned int echoStringLen;
    int sock;
    char *echoString = {"Hello there!"};
    char echoBuffer[RCVBUFSIZE];    
    struct sockaddr_in echoServAddr; 

    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        dieWithError("socket() failed");

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family      = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(ip);
    echoServAddr.sin_port        = htons(echoServPort); 

    if(connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        dieWithError("connect() failed");

    echoStringLen = sendFunc(&sock, echoString);

    receiveFunc(&sock, echoBuffer, echoStringLen);

    close(sock);
}

void consoleCommand() {
    printf("\tconsoleCommand\n");
}

void helpListCommand(char (*commands)[12]) {
    printf("\n");
    
    for(int i = 0; i < COMMAND_COUNTER; i++) {
        printf("%s\t\t", commands[i]);
        
        if(i % 5 == 0)
            printf("\n");
    }

    printf("\n\n");
}
