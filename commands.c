#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"
#include "internal.h"

//implements built-in cd command to change directory
int cmd_cd(char *path) {
    if (chdir(path) != 0) {
        perror("Error:");
        return 1;
    }
    return 0;
}

//implements built-in pwd command
int cmd_pwd(void) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("Error:");
        return 1;
    }
    return 0;
}

//prints contents of help file
int cmd_help(char *path) {
    FILE *f = fopen(path, "r");
    int c = fgetc(f);
    while(c != EOF) {
        printf("%c", c);
        c = fgetc(f);
    }
    printf("\n");
    fclose(f);
    return 0;
}

//execute program
int cmd_exec(char **args, int argv) {
    int i, n = 1, wait_flag = 0; //n = number of processes we have to make
    //wait_flag = if this is set to 1 then the process should wait for all background processes to finish
    int pipe_index[argv]; //track the index of the '|' symbol. yes this is dirty
    pipe_index[0] = 0; //pipe_index[i] is the index that the args for process i begin
    if (argv == 0) { //there are no arguments to work with
        return 1;
    }

    if (strcmp(args[argv-1], "&") == 0) { //wait flag
        wait_flag = 1;
    }

    for (i = 0; i < argv; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_index[n] = i+1; //pipe_index[i] is the index that the args for process i begin
            n++;
        }
    }

    int pipe_arr[n-1][2];
    for (i = 0; i < (n - 1) && n > 1; i++) {
        pipe(pipe_arr[i]);
    }

    int pid[n];
    for (i = 0; i < n; i++) {
        if ((pid[i] = fork()) < 0) {
            perror("Error: ");
            return 1;
        }
        //process i changes stdin and stdout
        if (pid[i] == 0) { //child
            for (int j = 0; j < argv; j++) {
                if (strcmp(args[j], "<") == 0) {
                    redirect(args[j + 1], 0);
                } else if(strcmp(args[j], ">") == 0)  {
                    redirect(args[j + 1], 1);
                }

            }

            if (n > 1) {
                if (i > 0) {
                    dup2(pipe_arr[i - 1][0], 0); //stdin
                }
                if (i < (n - 1)) {
                    dup2(pipe_arr[i][1], 1); //stdout
                }

                for (int x = 0; x < (n - 1); x++) {
                    close(pipe_arr[x][0]);
                    close(pipe_arr[x][1]);
                }
            }

            //we iterate through and set all special characters to NULL
            for (int x = 0; x<argv; x++) {
                if (strcmp(args[x], "<") == 0 || strcmp(args[x], ">") == 0 || strcmp(args[x], "|") == 0 || strcmp(args[x], "&") == 0) {
                    args[x] = NULL;
                }
            }
            if (execv(pathbuild(args[pipe_index[i]]), (args + pipe_index[i])) == -1) {
                perror("Error");
                return 1;
            }
        }
    }
    for (i = 0; i < n-1; i++) {
        wait(NULL);
        if(n > 1) {
            close(pipe_arr[i - 1][1]);
            close(pipe_arr[i][0]);
            close(pipe_arr[i - 1][0]);
            close(pipe_arr[i][1]);
        }
    }

    if (wait_flag == 1) {
        while(wait(NULL) > 0);
    }
    return 0;
}