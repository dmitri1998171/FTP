#include "extentions.h"
#include "commands.h"

int main(int argc, char *argv[]) {
    int authChecker = 0, dataChecker = 0;
    int sock, fileSock;
    char *sndArg;
    char command[15];
    char path[RCVBUFSIZE];
    char echoBuffer[RCVBUFSIZE];
    char commands[COMMAND_COUNTER][12] = {"!", "?", "ascii", "binary", "bye", "cd", "cdup", "close", "delete", "dir", "exit", "get", "hash", "help", "lcd", "ls", "mdelete", "mdir", "mget", "mkdir", "mls", "mput", "open", "passive", "put", "pwd", "rename", "restart", "reset", "recv", "rstatus", "rmdir", "send", "size", "status", "sendport", "quit", "disconnect"};
    
    if(argc == 2) 
        ValidateConnection(&sock, &fileSock, argv[1], echoBuffer, &authChecker, &dataChecker);
    if (argc != 2) {
        fprintf(stderr, "Usage:  %s [Server IP]\n", argv[0]);
        exit(1);
    }

    printf("\n");
    
    chdir(".");
    getcwd(path, RCVBUFSIZE);

    while(1) {
        printf("ftp> ");
        fgets_wrapper(command, RCVBUFSIZE - 1, stdin);

        parseCommandLine(command, &sndArg);
            
        if(checkTheCommand(commands, command)) {
            if(!strcmp(command, "?") || !strcmp(command, "help"))
                helpListCommand(commands);
            if(!strcmp(command, "bye") || !strcmp(command, "exit") || !strcmp(command, "quit")) {
                if(authChecker) 
                    disconnectFunc(&sock, &fileSock, echoBuffer);
                    
                exit(0);
            }
            if(!strcmp(command, "open")) {
                char ip[20];
                
                printf("(to) ");
                scanf("%s", ip);

                ValidateConnection(&sock, &fileSock, ip, echoBuffer, &authChecker, &dataChecker);
            }
        
            if(authChecker) {
                if(!strcmp(command, "cd"))
                    cdCommand(&sock, echoBuffer, sndArg);
                if(!strcmp(command, "disconnect") || !strcmp(command, "close")) {
                    disconnectFunc(&sock, &fileSock, echoBuffer);
                    authChecker = 0;
                }
                if(!strcmp(command, "get"))
                    getFunc(&sock, &fileSock, echoBuffer, sndArg);
                if(!strcmp(command, "ls"))
                    CommandFunc("LIST", &sock, &fileSock, echoBuffer);
                // if(!strcmp(command, "mkdir"))
                if(!strcmp(command, "pwd"))
                    CommandFunc("PWD", &sock, &fileSock, echoBuffer);
                // if(!strcmp(command, "rename"))
                // if(!strcmp(command, "recv"))
                // if(!strcmp(command, "rmdir"))
                // if(!strcmp(command, "send"))
                // if(!strcmp(command, "sendport"))
                // if(!strcmp(command, "size"))
                // if(!strcmp(command, "status"))
            }
            else
                printf("Not connected\n");        
        } 
        else
            printf("Invalid command\n");
    }

    return 0;
}