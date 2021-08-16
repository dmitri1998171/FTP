#include "commands.h"
#include "extentions.h"

#define RCVBUFSIZE 32   

void openCommand(char *ip) {
    unsigned short echoServPort = 1026;     
    unsigned int echoStringLen;      
    int sock;                        
    int bytesRcvd = 0, totalBytesRcvd = 0;   
    char *echoString = {"Hello there!"};
    char echoBuffer[RCVBUFSIZE];     
    struct sockaddr_in echoServAddr; 

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        dieWithError("socket() failed");

    memset(&echoServAddr, 0, sizeof(echoServAddr));     
    echoServAddr.sin_family      = AF_INET;             
    echoServAddr.sin_addr.s_addr = inet_addr(ip);       
    echoServAddr.sin_port        = htons(echoServPort); 

    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        dieWithError("connect() failed");

    echoStringLen = strlen(echoString);          

    if (send(sock, echoString, echoStringLen, 0) != echoStringLen)
        dieWithError("send() sent a different number of bytes than expected");

    printf("Received: ");          
    while (totalBytesRcvd < echoStringLen)
    {
        if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
            dieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd;   
        echoBuffer[bytesRcvd] = '\0';  
        printf("%s", echoBuffer);      
    }

    printf("\n");    

    close(sock);
}

void consoleCommand() {
    printf("\tconsoleCommand\n");
}

void helpListCommand() {
    
}
