#include "resolve.h"
#include "commands.h"
#include <string.h>
#include <stdlib.h>

//resolves tokenized input to command and executes appropriate function.
//return 0 for successful call; 1 for command not existent.
int resolve(char **args, char *help_file, int argv) {
    if (args[0] == NULL) {
        exit(1);
    } else if (strcmp(args[0], "exit") == 0) {
        exit(0);
    } else if(strcmp(args[0], "help") == 0) {
        cmd_help(help_file);
        return 0;
    } else if(strcmp(args[0], "pwd") == 0) {
        cmd_pwd();
        return 0;
    } else if(strcmp(args[0], "cd") == 0) {
        cmd_cd(args[1]);
        return 0;
    } else if(strcmp(args[0], "wait") == 0) {
        strcpy(args[0], "&");
        cmd_exec(args, 1);
        return 0;
    } else {
        if (cmd_exec(args, argv) != 0) {
            return 1;
        }
        return 0;
    }
}