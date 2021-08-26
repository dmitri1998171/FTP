#include "extentions.h"
#include "commands.h"

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

void handshakeFunc(int *sock, char *echoString, char* echoBuffer) {
    sendFunc(sock, "SYN");
    receiveFunc(sock, echoBuffer);

    if(!strcmp(echoBuffer, "SYN+ACK"))
        sendFunc(sock, "ACK");

    receiveFunc(sock, echoBuffer);
    
    if(!strcmp(echoBuffer, "225"))
        printf("Handshake complete!\n\n");
}

void openCommand(int *sock, char *ip, char *echoBuffer) {
    unsigned short echoServPort = 1026;
    char *echoString = { "Hello there!" };
    struct sockaddr_in echoServAddr; 

    if((*sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        dieWithError("socket() failed");

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(ip);
    echoServAddr.sin_port = htons(echoServPort);

    if(connect(*sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        dieWithError("connect() failed");

    // handshakeFunc(&sock, echoString, echoBuffer);

    receiveFunc(sock, echoBuffer);
    
    if(!strcmp(echoBuffer, "225"))
        printf("Connection complete!\n\n");

    // close(sock);
}

void disconnectFunc(int *sock, char *echoBuffer) {
    sendFunc(sock, "QUIT");

    receiveFunc(sock, echoBuffer);
    
    if(!strcmp(echoBuffer, "221"))
        printf("Disconnect complete!\n\n");

    close(*sock);
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
