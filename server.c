#include "extentions.h" 

#define MAXPENDING 5 

int authLoginPasswrd(int *sock) {
    char* echoBuffer;
    receiveFunc(sock, echoBuffer);

    char str[RCVBUFSIZE];
    char* login;
    char* passwrd;
    FILE* fd = fopen("auth.txt", "r");

    while (fgets(str, RCVBUFSIZE, fd) != NULL) {
        login = strtok(str, " ");
        passwrd = strtok(NULL, " ");

        if(!strcmp(echoBuffer, login)) {
            sendFunc(sock, "331");
            receiveFunc(sock, echoBuffer);

            if(!strcmp(echoBuffer, passwrd)) {
                sendFunc(sock, "230");
                return 1;
            }
        }        
    }

    sendFunc(sock, "530");
    close(*sock);
    fclose(fd);
    return 0;
}

int CommandFunc(char* command, char buffer[]) {
    FILE *lsptr;
    char localBuffer[RCVBUFSIZE];

    buffer[0] = '\0';

    lsptr = popen(command, "r");

    if(lsptr) {
        while(fgets(localBuffer, RCVBUFSIZE - 1, lsptr)) 
            strcat(buffer, localBuffer);

        pclose(lsptr);
        return 0;
    }else
        printf("Error: command not work!\n");

    return 1;
}

void createConnection(int *servSock, unsigned short echoServPort) {
    struct sockaddr_in echoServAddr; 
    
    if ((*servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        dieWithError("socket() failed");
      
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    echoServAddr.sin_port = htons(echoServPort); 

    const int optval = 1;
    setsockopt(*servSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if (bind(*servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        dieWithError("bind() failed");
}

int checkTheFile(char *filename) {
    char localBuffer[RCVBUFSIZE];

    CommandFunc("ls", localBuffer);
    for(int i = 0; i < strlen(localBuffer); i++)
        if(!strstr(filename, localBuffer))
            return 0;

    return 1;
}

int main(int argc, char *argv[]) {
    char *path = "./";
    char *filename;
    int result = 0;
    int servSock, clntSock, servSockData, clntSockData;
    char dataBuffer[RCVBUFSIZE];
    char echoBuffer[RCVBUFSIZE];
    struct sockaddr_in echoClntAddr, echoClntAddrData; 
    unsigned short echoServPort;
    unsigned int clntLen, clntLenData;
    unsigned long fileSize;

    if (argc != 2) {
        fprintf(stderr, "Usage:  %s <Server Port>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  

    createConnection(&servSock, echoServPort);

    if (listen(servSock, MAXPENDING) < 0)
        dieWithError("listen() failed");

    createConnection(&servSockData, echoServPort + 21);

    if (listen(servSockData, MAXPENDING) < 0)
        dieWithError("listen2() failed");

    while(1) {
        clntLen = sizeof(echoClntAddr);
        if((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0) {
            sendFunc(&clntSock, "426");
            dieWithError("accept() failed");
        }
        
        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
        sendFunc(&clntSock, "220");
        
        if(authLoginPasswrd(&clntSock)) {
            clntLenData = sizeof(echoClntAddrData);
            if((clntSockData = accept(servSockData, (struct sockaddr *) &echoClntAddrData, &clntLenData)) < 0) {
                sendFunc(&clntSock, "426");
                dieWithError("accept() failed"); 
            }

            usleep(500 * 1000);
            sendFunc(&clntSockData, "220");

            while(1) {
                receiveFunc(&clntSock, echoBuffer);

                if(!strcmp(echoBuffer, "LIST")) {
                    result = CommandFunc("ls", dataBuffer);
                    sendResult(&clntSock, result);
                    sendFunc(&clntSockData, dataBuffer);
                }

                if(!strcmp(echoBuffer, "RNFR")) {                    
                    char oldName[RCVBUFSIZE];

                    receiveFunc(&clntSock, echoBuffer);     // Получаем старое имя файла
                    strcpy(oldName, echoBuffer);

                    result = checkTheFile(oldName);      // Проверка что файл существует
                    sendResult(&clntSock, result);

                    if(!result) {
                        receiveFunc(&clntSock, echoBuffer);     // Получаем новое имя файла
                        
                        result = rename(oldName, echoBuffer);
                        sendResult(&clntSock, result);       // Проверка результата
                    }
                }

                if(!strcmp(echoBuffer, "MKD")) {
                    receiveFunc(&clntSock, echoBuffer);

                    result = mkdir(echoBuffer, S_IRWXU|S_IRGRP|S_IXGRP);
                    sendResult(&clntSock, result);
                }

                if(!strcmp(echoBuffer, "RMD")) {
                    receiveFunc(&clntSock, echoBuffer);

                    result = rmdir(echoBuffer);
                    sendResult(&clntSock, result);
                }

                if(!strcmp(echoBuffer, "PWD")) {
                    result = CommandFunc("pwd", dataBuffer);
                    sendResult(&clntSock, result);
                    sendFunc(&clntSockData, dataBuffer);
                }

                if(!strcmp(echoBuffer, "CD")) {
                    receiveFunc(&clntSock, echoBuffer);

                    result = chdir(echoBuffer);
                    sendResult(&clntSock, result);
                }

                if(!strcmp(echoBuffer, "GET")) {
                    receiveFunc(&clntSock, echoBuffer);     // Получаем имя файла

                    result = checkTheFile(echoBuffer);      // Проверка что файл существует
                    sendResult(&clntSock, result);

                    if(!result) {
                        char fileSiz[RCVBUFSIZE];

                        fileSize = readFile(echoBuffer, dataBuffer);  // Читаем файл

                        snprintf(fileSiz, sizeof(fileSiz), "%lu", fileSize);
                        sendFunc(&clntSock, fileSiz);           // Отправляем размер
                        
                        usleep(500 * 1000);

                        sendFileFunc(&clntSock, echoBuffer);
                    }
                }

                if(!strcmp(echoBuffer, "QUIT")) {
                    sendFunc(&clntSock, "221");
                    close(clntSock);
                    break;
                }
            }
        }
    }
    
    close(clntSock);
    return 0;
}