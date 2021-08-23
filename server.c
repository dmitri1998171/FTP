#include "extentions.h" 

#define MAXPENDING 5    

int main(int argc, char *argv[]) {
    char echoBuffer[RCVBUFSIZE];     
    int recvMsgSize;                 
    int servSock;                    
    int clntSock;                    
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

        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, 
                               &clntLen)) < 0)
            dieWithError("accept() failed");

        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

        if ((recvMsgSize = recv(clntSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
            dieWithError("recv() failed");

        printf("%s\n", echoBuffer);

        // while (recvMsgSize > 0)
        // {
            if (send(clntSock, echoBuffer, recvMsgSize, 0) != recvMsgSize)
                dieWithError("send() failed");

            // if ((recvMsgSize = recv(clntSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
            //     dieWithError("recv() failed");
        // }

        close(clntSock);
    }

    return 0;
}