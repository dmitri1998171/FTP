#include "extentions.h"
#include "commands.h"

int main(int argc, char *argv[]) {
    int authChecker = 0;
    int sock, fileSock;
    char *sndArg;
    char command[15];
    char path[RCVBUFSIZE];
    char echoBuffer[RCVBUFSIZE];
    // char *fstCommand, *sndCommand;
    char commands[COMMAND_COUNTER][12] = {"!", "?", "ascii", "binary", "bye", "cd", "cdup", "close", "delete", "dir", "exit", "get", "hash", "help", "lcd", "ls", "mdelete", "mdir", "mget", "mkdir", "mls", "mput", "open", "passive", "put", "pwd", "rename", "restart", "reset", "recv", "rstatus", "rmdir", "send", "size", "status", "sendport", "quit", "disconnect"};
    
    if(argc > 1) {
        if(checkIP(argv[1])) {
            openCommand(&sock, argv[1], PORT);
            authChecker = checkConnection(&sock, argv[1], echoBuffer);
            openCommand(&fileSock, argv[1], PORT + 21);
        }
    }
    if (argc > 2) {
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
            if(!strcmp(command, "!"))
                consoleCommand();
            if(!strcmp(command, "?") || !strcmp(command, "help"))
                helpListCommand(commands);
            if(!strcmp(command, "bye") || !strcmp(command, "exit") || !strcmp(command, "quit"))
                return 0;
            if(!strcmp(command, "open")) {
                char ip[20];
                
                printf("(to) ");
                scanf("%s", ip);

                if(checkIP(ip)) {
                    openCommand(&sock, ip, PORT);
                    authChecker = checkConnection(&sock, ip, echoBuffer);
                    openCommand(&fileSock, ip, PORT + 21);
                }
                else
                    printf("ftp: %s: Temporary failure in name resolution\n", ip);
            }

            // if(!strcmp(command, "ascii"))
            // if(!strcmp(command, "binary"))
        
            if(authChecker) {
                if(!strcmp(command, "cd"))
                    cdCommand(sndArg);
                // if(!strcmp(command, "cdup"))
                // if(!strcmp(command, "delete"))
                if(!strcmp(command, "disconnect") || !strcmp(command, "close")) {
                    disconnectFunc(&sock, echoBuffer);
                    authChecker = 0;
                }
                // if(!strcmp(command, "dir"))
                // if(!strcmp(command, "get"))
                // if(!strcmp(command, "hash"))
                // if(!strcmp(command, "lcd"))
                if(!strcmp(command, "ls"))
                    CommandFunc("LIST", &sock, &fileSock, echoBuffer);
                // if(!strcmp(command, "mdelete"))
                // if(!strcmp(command, "mdir"))
                // if(!strcmp(command, "mget"))
                // if(!strcmp(command, "mkdir"))
                // if(!strcmp(command, "mls"))
                // if(!strcmp(command, "mput"))
                // if(!strcmp(command, "passive"))
                // if(!strcmp(command, "put"))
                if(!strcmp(command, "pwd"))
                    CommandFunc("PWD", &sock, &fileSock, echoBuffer);
                // if(!strcmp(command, "rename"))
                // if(!strcmp(command, "restart"))
                // if(!strcmp(command, "reset"))
                // if(!strcmp(command, "recv"))
                // if(!strcmp(command, "rstatus"))
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