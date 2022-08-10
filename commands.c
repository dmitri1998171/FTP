#include "extentions.h"
#include "commands.h"

int checkTheCommand(char (*commands)[12], char *command) {
    int command_checker = 0;

    // printf("command_4: %s\n", command);

    for(int i = 0; i < COMMAND_COUNTER; i++) {
        if(!strcmp(commands[i], command)) {
            command_checker = 1;
            break;
        }
    }

    return command_checker;
}

int authLogin(int *sock, char* echoBuffer) {
    char username[RCVBUFSIZE];

    printf("Name: ");
    scanf("%s", username);

    sendFunc(sock, username);
    receiveFunc(sock, echoBuffer);
    
    if(!strcmp(echoBuffer, "331")) {
        printf("331 Please specify the password\n");
        if(authPasswrd(sock, echoBuffer))
            return 1;
    }

    printf("530 Login incorrect\nLogin failed\n\n");
    return 0;
}

int authPasswrd(int *sock, char* echoBuffer) {
    char passwrd[RCVBUFSIZE];

    printf("Password: ");
    scanf("%s", passwrd);

    sendFunc(sock, passwrd);
    receiveFunc(sock, echoBuffer);

    if(!strcmp(echoBuffer, "230")) {
        printf("230 Login successful\n");
        return 1;
    }
    else {
        printf("530 Password incorrect\n");
        close(*sock);
        return 0;
    }
}

int checkConnection(int *sock, char *ip, char *echoBuffer) {
    receiveFunc(sock, echoBuffer);
    
    if(!strcmp(echoBuffer, "220")) {
        printf("Connected to %s\n\n", ip);
        return 1;
    }
    else
        dieWithError("Failed to connect!");

    return 0;
}

void ValidateConnection(int *sock, int *fileSock, char *ip, char *echoBuffer, int *authChecker, int *dataChecker) {
    if(checkIP(ip)) {
        openCommand(sock, ip, PORT);
        checkConnection(sock, ip, echoBuffer);
        *authChecker = authLogin(sock, echoBuffer);

        openCommand(fileSock, ip, PORT + 21);
        *dataChecker = checkConnection(fileSock, ip, echoBuffer);
    }
    else
        printf("ftp: %s: Temporary failure in name resolution\n", ip);
}

void openCommand(int *sock, char *ip, unsigned short echoServPort) {
    struct sockaddr_in echoServAddr; 

    if((*sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        dieWithError("socket() failed");

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(ip);
    echoServAddr.sin_port = htons(echoServPort);

    if(connect(*sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        dieWithError("connect() failed");
}

void disconnectFunc(int *sock, int *fileSock, char *echoBuffer) {
    sendFunc(sock, "QUIT");

    receiveFunc(sock, echoBuffer);
    
    if(!strcmp(echoBuffer, "221"))
        printf("Disconnect complete!\n\n");

    close(*sock);
    close(*fileSock);
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

void getFile(int *fileSock, char *filename) {
    int run = 1;
    char localBuffer[RCVBUFSIZE];
    // FILE *file = fopen(filename, "w");

    receiveFunc(fileSock, localBuffer);

    // while (run) {
    //     receiveFunc(fileSock, localBuffer);
    //     fprintf(file, "%s\n", localBuffer);
    // }
}

void cdCommand(int *sock, char *echoBuffer, char *path) {
    sendFunc(sock, "CD");
    usleep(500*1000);
    sendFunc(sock, path);
    receiveFunc(sock, echoBuffer);
}

void CommandFunc(char *command, int *sock, int *fileSock, char *echoBuffer) {
    char localBuffer[RCVBUFSIZE];
    // strcpy(echoBuffer, command);

    sendFunc(sock, command);
    receiveFunc(sock, echoBuffer);

    if(!strcmp(echoBuffer, "226")) 
        receiveFunc(fileSock, localBuffer);
    
    if(!strcmp(echoBuffer, "451")) 
        printf("Local error!\n");
}

int getFunc(int *sock, int *fileSock, char *echoBuffer, char *filename) {
    unsigned long fileSize = 0;

    sendFunc(sock, "GET");
    
    usleep(500 * 1000);

    sendFunc(sock, filename);           // Отправка имени на сервер
    receiveFunc(sock, echoBuffer);      // Проверка существует ли такой файл

    if(!strcmp(echoBuffer, "226")) 
        receiveFunc(sock, echoBuffer);  // Получаем размер файла
    
    if(!strcmp(echoBuffer, "451")) {
        printf("Local error!\n");
        return 1;
    }

    fileSize = atol(echoBuffer);
    receiveFile(sock, filename);

    return 0;
}
