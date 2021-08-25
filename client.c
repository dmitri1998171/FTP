#include "extentions.h"
#include "commands.h"

int main(int argc, char *argv[]) {
    int run = 1;
    char command[15];
    char commands[COMMAND_COUNTER][12] = {"!", "?", "ascii", "binary", "bye", "cd", "cdup", "close", "delete", "dir", "exit", "get", "hash", "help", "lcd", "ls", "mdelete", "mdir", "mget", "mkdir", "mls", "mput", "open", "passive", "put", "pwd", "rename", "restart", "reset", "recv", "rstatus", "rmdir", "send", "size", "status", "sendport", "quit", "disconnect"};
    
    if(argc > 1) {
        if(checkIP(argv[1]))
            openCommand(argv[1]);
    }
    if (argc > 2) {
        fprintf(stderr, "Usage:  %s [Server IP]\n", argv[0]);
        exit(1);
    }

    printf("\n");
    
    while(run) {
        printf("ftp> ");
        scanf("%s", command);

        if(checkTheCommand(commands, command)) {
            if(!strcmp(command, "!"))
                consoleCommand();
            if(!strcmp(command, "?") || !strcmp(command, "help"))
                helpListCommand(commands);
            // if(!strcmp(command, "ascii"))
            // if(!strcmp(command, "binary"))
            // if(!strcmp(command, "cd"))
            // if(!strcmp(command, "cdup"))
            // if(!strcmp(command, "close"))
            // if(!strcmp(command, "delete"))
            // if(!strcmp(command, "disconnect"))
            // if(!strcmp(command, "dir"))
            // if(!strcmp(command, "get"))
            // if(!strcmp(command, "hash"))
            // if(!strcmp(command, "lcd"))
            // if(!strcmp(command, "ls"))
            // if(!strcmp(command, "mdelete"))
            // if(!strcmp(command, "mdir"))
            // if(!strcmp(command, "mget"))
            // if(!strcmp(command, "mkdir"))
            // if(!strcmp(command, "mls"))
            // if(!strcmp(command, "mput"))
            if(!strcmp(command, "open")) {
                char ip[20];
                
                printf("(to) ");
                scanf("%s", ip);

                if(checkIP(ip))
                    openCommand(ip);
                else
                    printf("ftp: %s: Temporary failure in name resolution\n", ip);
            }
            // if(!strcmp(command, "passive"))
            // if(!strcmp(command, "put"))
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
            if(!strcmp(command, "bye") || !strcmp(command, "exit") || !strcmp(command, "quit"))
                run = 0;            
        } 
        else
            printf("Invalid command\n");
    }

    return 0;
}