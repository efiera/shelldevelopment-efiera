#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "helpers.h"
#include "resolve.h"
#include "internal.h"

#define MAX_ARG_LENGTH 1024 // the maximum length of line the user may enter

int main(void) {
    char *line = malloc(sizeof(char) * MAX_ARG_LENGTH);
    char help_path[PATH_MAX];
    //save absolute path to help file in case user requests it after cd
    realpath("help.txt", help_path);
    while(1) { //this needs to get changed later
        char **args;
        size_t len = 0;
        //printf("> ");
        //accept user input in while loop
        while (len <= 1) {
            fgets(line, MAX_ARG_LENGTH, stdin);
            //replace newline with null
            len = strlen(line);
            if (len > 0 && line[len - 1] == '\n') {
                line[len - 1] = '\0';
            }
        }

        args = parse(line, " ");
        int argv = arr_len(args);

        if (resolve(args, help_path, argv) == 1) {
            printf("Command not recognized.\n");
        }
    }
    free(line);
    return 0;
}