#include "extentions.h" 

#define MAXPENDING 5 

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
}

int main(int argc, char *argv[]) {
    char echoBuffer[RCVBUFSIZE];
    char tmp[RCVBUFSIZE];
    int authChecker = 0;
    int recvMsgSize;
    int servSock, clntSock;
    struct sockaddr_in echoServAddr; 
    struct sockaddr_in echoClntAddr; 
    unsigned short echoServPort;
    unsigned int clntLen;

    struct auth_struct logPassList[3] = {"admin",     "admin", 
                                         "anonymous", "",
                                         "dmitry",    "123"};

    if (argc != 2) {
        fprintf(stderr, "Usage:  %s <Server Port>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  

    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        dieWithError("socket() failed");
      
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;   
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    echoServAddr.sin_port = htons(echoServPort); 

    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        dieWithError("bind() failed");

    if (listen(servSock, MAXPENDING) < 0)
        dieWithError("listen() failed");

    while(1) {
        clntLen = sizeof(echoClntAddr);

        if((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
            dieWithError("accept() failed");
        else {
            printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
            sendFunc(&clntSock, "220");
            authChecker = authLogin(&clntSock, echoBuffer, logPassList);
        }

        if(authChecker) {
            while(1) {
                receiveFunc(&clntSock, echoBuffer);

                if(!strcmp(echoBuffer, "LIST")) {
                    sendFunc(&clntSock, "150");
                    
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