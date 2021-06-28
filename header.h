#include <dirent.h>
#include <grp.h>
#include <math.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#ifndef MAXLEN
#define MAXLEN 100
#endif
void clear(char *args[]);
int readCommand(char *args[], int *argv);
int readFile(const char *path);
int processCommand(char *args[], int *argv);
int printFile(char *path);
