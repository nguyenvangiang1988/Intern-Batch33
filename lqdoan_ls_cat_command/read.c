#include "header.h"

void clear(char *args[]) {
  for (int i = 0; i < MAXLEN / 2 + 1; ++i) {
    args[i] = NULL;
  }
}

int readCommand(char *args[], int *argv) {
  clear(args);
  char command[MAXLEN];
  fgets(command, MAXLEN, stdin);
  command[strlen(command) - 1] = '\0';
  if (strcmp(command, "exit") == 0)
    return 0;
  else {
    const char delimiter[2] = " ";
    char *token = strtok(command, delimiter);
    *argv = 0;
    while (token != NULL) {
      args[*argv] = strdup(token);
      ++*argv;
      token = strtok(NULL, delimiter);
    }
  }
  args[*argv] = NULL;
  return 1;
}

int printFile(char *path) {
  FILE *file;
  file = fopen(path, "r");
  if (file == NULL) {
    printf("File not found.\n");
    return 1;
  }
  char c;
  while ((c = fgetc(file)) != EOF) printf("%c", c);
  fclose(file);
  return 0;
}
