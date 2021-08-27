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

void authLogin(int *sock, char* echoBuffer) {
    char username[RCVBUFSIZE];

    printf("Name: ");
    scanf("%s", username);

    sendFunc(sock, username);
    receiveFunc(sock, echoBuffer);

    if(!strcmp(echoBuffer, "331")) {
        printf("331 Please specify the password\n");
        authPasswrd(sock, echoBuffer);
    }
    else
        printf("530 Login incorrect\nLogin failed\n\n");
}

void authPasswrd(int *sock, char* echoBuffer) {
    char passwrd[RCVBUFSIZE];

    printf("Password: ");
    scanf("%s", passwrd);

    sendFunc(sock, passwrd);
    receiveFunc(sock, echoBuffer);

    if(!strcmp(echoBuffer, "230")) 
        printf("230 Login successful\n");
    else {
        printf("530 Password incorrect\n");
        close(*sock);
    }
}

void checkConnection(int *sock, char *ip, char *echoBuffer) {
    receiveFunc(sock, echoBuffer);
    
    if(!strcmp(echoBuffer, "220")) {
        printf("Connected to %s\n\n", ip);
        authLogin(sock, echoBuffer);
    }
}

void openCommand(int *sock, char *ip, unsigned short echoServPort) {
    struct sockaddr_in echoServAddr; 

    if((*sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        dieWithError("socket() failed");

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(ip);
    echoServAddr.sin_port = htons(echoServPort);

    if(connect(*sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        dieWithError("connect() failed");
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

void lsCommand(int *sock, char *echoBuffer) {
    strcpy(echoBuffer, "LIST");
    sendFunc(sock, echoBuffer);
    receiveFunc(sock, echoBuffer);
}
