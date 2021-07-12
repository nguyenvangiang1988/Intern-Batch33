#include "header.h"
int main() {
  int running = 1, argv;
  char *args[MAXLEN / 2 + 1];
  while (running) {
    argv = 0;
    printf("\033[0;32m");  // set color
    printf("cmd>");
    fflush(stdout);
    printf("\033[0m");  // reset color
    running = readCommand(args, &argv);

    int status;
    if (!running) break;
    /*
    for (int i = 0; i < argv; ++i)
            printf("input: %s \n", args[i]);
    */
    if (argv > 0) status = processCommand(args, &argv);
    if (status == 1) printf("Command error.\n");
    for (int i = 0; i < argv; i += 1)
  	free((char*) args[i]);
  }

  return 0;
}
