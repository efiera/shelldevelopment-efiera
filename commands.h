#pragma once

int cmd_cd(char *path);
int cmd_pwd(void);
int cmd_help(char *path);
int cmd_exec(char **args, int argv);
