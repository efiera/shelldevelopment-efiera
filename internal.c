#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "helpers.h"
#include <fcntl.h>

//returns the number of elements of an array of strings. MUST BE NULL TERMINATED.
int arr_len(char **arr) {
    int i = 0;
    while(arr[i] != NULL) {
        i++;
    }
    return i; //excludes NULL
}

//parses the path variable into an array for later path resolution
char** get_path() {
    const char *path = getenv("PATH"); //holds PATH variable
    char *copyPATH = (char *) malloc(strlen(path) + 1);
    strcpy(copyPATH,path); //copy to another string for strtok
    return parse(copyPATH, ":");
}

//handles redirection.
//file is the filename to be redirected.
//mode is whether to redirect stdin(0) or stdout(1).
void redirect(char *filename, int mode) {
    int fd = 0;
    if(mode == 0) { //send stdin from a file
        fd = open(filename, O_RDONLY);
        dup2(fd, mode);
        close(fd);
    } else if(mode == 1) { //send stdout to a file
        fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0777);
        dup2(fd, mode);
        close(fd);
    }
}

char* pathbuild(char *arg) {
    char *pathname = malloc(sizeof(char) * 200); //the full path that will be checked for valid file
    if (access(arg, F_OK) != 0) {
        int i = 0;
        char **pathvar; //the path variable, as an array
        pathvar = get_path();
        int pathvar_len = arr_len(pathvar);
        while (i < pathvar_len && access(pathname, F_OK) != 0) {
            strcpy(pathname, pathvar[i++]);
            strcat(pathname, "/");
            strcat(pathname, arg);
        }
        if (access(pathname, F_OK) == 0) {
            return pathname;
        }
        return NULL;
    }
    strcpy(pathname, arg);
    return pathname;
}