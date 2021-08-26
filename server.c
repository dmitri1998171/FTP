#include "extentions.h" 

#define MAXPENDING 5 

int main(int argc, char *argv[]) {
    char echoBuffer[RCVBUFSIZE];
    char tmp[RCVBUFSIZE];
    int recvMsgSize;
    int servSock, clntSock;
    struct sockaddr_in echoServAddr; 
    struct sockaddr_in echoClntAddr; 
    unsigned short echoServPort;
    unsigned int clntLen;

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
            sendFunc(&clntSock, "225");
        }

        while(1) {
            receiveFunc(&clntSock, echoBuffer);

            if(!strcmp(echoBuffer, "QUIT")) {
                sendFunc(&clntSock, "221");
                close(clntSock);
                break;
            }
        }
    }
    
    close(clntSock);
    return 0;
}