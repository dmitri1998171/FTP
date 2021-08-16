#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define COMMAND_COUNTER 38

void dieWithError(char *str) {
    printf("%s\n", str);
    exit(1);
}

int checkIP(char *str) {
    int octetCounter = 0;
    char *token, *last;
    char *ip = strtok(str, ":");

    token = strtok_r(str, ".", &last);
    while(token != NULL) {
        if(atoi(token) > 255)
            dieWithError("Invalid IP!");
        token = strtok_r(NULL, ".", &last);
        octetCounter++;
    }
    
    if(octetCounter == 4) 
        return 1;
    else
        return 0;
}

void openCommand(char *ip) {
    printf("\topenCommand\n");
}

void consoleCommand() {
    printf("\tconsoleCommand\n");
}

void helpListCommand() {
    
}

int main(int argc, char *argv[]) {
    int run = 1;
    int command_checker = 0;
    char command[15];
    char commands[COMMAND_COUNTER][12] = {"!", "?", "ascii", "binary", "bye", "cd", "cdup", "close", "delete", "disconnect", "dir", "exit", "get", "hash", "help", "lcd", "ls", "mdelete", "mdir", "mget", "mkdir", "mls", "mput", "open", "passive", "put", "pwd", "rename", "restart", "reset", "recv", "rstatus", "rmdir", "send", "sendport", "size", "status", "quit"};
    
    if(argc > 1) {
        if(checkIP(argv[1]))
            openCommand(argv[1]);
    }
    printf("\n");
    
    while(run) {
        printf("ftp> ");
        scanf("%s", command);

        for (int i = 0; i < COMMAND_COUNTER; i++) {
            if(!strcmp(commands[i], command)) {
                command_checker = 1;
                break;
            }
        }

        if(command_checker) {   
            if(!strcmp(command, "open"))
                openCommand("1");
            if(!strcmp(command, "bye") || !strcmp(command, "exit"))
                run = 0;
            
            command_checker = 0;
        } else
            printf("Invalid command\n");
    }

    return 0;
}