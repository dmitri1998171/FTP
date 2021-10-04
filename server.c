#include "extentions.h" 

#define MAXPENDING 5 
#define TMP_FILE ".tmp.txt"

int servSock, clntSock, servSockData, clntSockData;

struct auth_struct {
    char login[RCVBUFSIZE];
    char passwrd[RCVBUFSIZE];
};

int authPasswrd(int *sock, char* echoBuffer, char *passwrd) {
    receiveFunc(sock, echoBuffer);

    if(!strcmp(echoBuffer, passwrd)) {
        sendFunc(sock, "230");
        return 1;
    }
    else {
        sendFunc(sock, "530");
        close(*sock);
        return 0;
    }
}

int authLogin(int *sock, char* echoBuffer, struct auth_struct *logPassList) {
    receiveFunc(sock, echoBuffer);

    for(int i = 0; i < 3; i++) {
        if(!strcmp(echoBuffer, logPassList[i].login)) {
            sendFunc(sock, "331");
            return authPasswrd(sock, echoBuffer, logPassList[i].passwrd);
        }
    }

    sendFunc(sock, "530");
    return 0;
}

void createConnection(int *servSock, unsigned short echoServPort) {
    struct sockaddr_in echoServAddr; 
    
    if ((*servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        dieWithError("socket() failed");
      
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    echoServAddr.sin_port = htons(echoServPort); 

    if (bind(*servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        dieWithError("bind() failed");
}

int lsCommand(char *path, int* clntSockData, char buffer[], int *bytesCounter) {
    FILE *lsptr;
    int fptr;
    char localBuffer[RCVBUFSIZE];

    // fptr = open(TMP_FILE, O_CREAT|O_RDWR,S_IREAD|S_IWRITE);

    // if(fptr) {
        // strcat(path, "/ls");
        // printf("path: %s\n", path);
        lsptr = popen("ls", "r");

        if(lsptr) {
            while(fgets(localBuffer, RCVBUFSIZE - 1, lsptr)) {
                // *bytesCounter = write(fptr, localBuffer, strlen(localBuffer));
                strcat(buffer, localBuffer);
            }

            pclose(lsptr);
            // close(fptr);
            return 0;
        }else
            printf("Error: ls command work!\n");

        return 2;
    // }else
    //     printf("Error: file open!\n");

    // return 1;
}

void signalListener(int sig) {
    system("rm -rf .tmp.txt");
    close(clntSock);
    close(clntSockData);
    exit(0);
}

void createConnection(int *servSock, unsigned short echoServPort) {
    struct sockaddr_in echoServAddr; 
    
    if ((*servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        dieWithError("socket() failed");
      
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    echoServAddr.sin_port = htons(echoServPort); 

    if (bind(*servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        dieWithError("bind() failed");
}

int main(int argc, char *argv[]) {
    char echoBuffer[RCVBUFSIZE];
    char tmp[RCVBUFSIZE];
    char *path = "./";
    int authChecker = 0;
    int recvMsgSize;
    struct sockaddr_in echoClntAddr, echoClntAddrData; 
    unsigned short echoServPort;
    unsigned int clntLen, clntLenData;

    struct auth_struct logPassList[3] = {"admin",     "admin", 
                                         "anonymous", "",
                                         "dmitry",    "123"};

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
        signal(SIGINT, signalListener);

        clntLen = sizeof(echoClntAddr);
        if((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
            dieWithError("accept() failed");
        
        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
        sendFunc(&clntSock, "220");
        authChecker = authLogin(&clntSock, echoBuffer, logPassList);

        if(authChecker) {
            while(1) {
                receiveFunc(&clntSock, echoBuffer);

                if(!strcmp(echoBuffer, "LIST")) {
                    sendFunc(&clntSock, "150");

                    clntLenData = sizeof(echoClntAddrData);
                    if((clntSockData = accept(servSockData, (struct sockaddr *) &echoClntAddrData, &clntLenData)) < 0)
                        dieWithError("accept() failed"); 

                    usleep(500 * 1000);
                    sendFunc(&clntSock, "220");

                    char localBuffer[RCVBUFSIZE];
                    int result, bytesCounter = 0;

                    result = lsCommand(path, &clntSockData, localBuffer, &bytesCounter);

                    printf("localBuffer: %s\n", localBuffer);

                    if(!result) {
                        sendFunc(&clntSock, "226");     // Успешно

                        // sendIntFunc(&clntSockData, &bytesCounter);  // Кол-во байт, которое будет отправлено
                        // usleep(500 * 1000);
                        sendFunc(&clntSockData, localBuffer);       // Непосретсвенно отправка полезной нагрузки
                    }
                    // else if(result == 1)
                    //     sendFunc(&clntSock, "450");     // Файл недоступен
                    else
                        sendFunc(&clntSock, "451");     // Операция прервана, ошибка
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