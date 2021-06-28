#include "header.h"

void displaylong(char *path) {
  struct tm *time;
  struct stat att;
  struct passwd *usr;
  struct group *grp;
  char line[1000] = "\0";

  stat(path, &att);
  usr = getpwuid(att.st_uid);
  grp = getgrgid(att.st_gid);
  time = localtime(&(att.st_mtime));

  if (S_ISDIR(att.st_mode))
    strcat(line, "d");
  else
    strcat(line, "-");
  if (att.st_mode & S_IRUSR)
    strcat(line, "r");
  else
    strcat(line, "-");
  if (att.st_mode & S_IWUSR)
    strcat(line, "w");
  else
    strcat(line, "-");
  if (att.st_mode & S_IXUSR)
    strcat(line, "x");
  else
    strcat(line, "-");
  if (att.st_mode & S_IRGRP)
    strcat(line, "r");
  else
    strcat(line, "-");
  if (att.st_mode & S_IWGRP)
    strcat(line, "w");
  else
    strcat(line, "-");
  if (att.st_mode & S_IXGRP)
    strcat(line, "x");
  else
    strcat(line, "-");
  if (att.st_mode & S_IROTH)
    strcat(line, "r");
  else
    strcat(line, "-");
  if (att.st_mode & S_IWOTH)
    strcat(line, "w");
  else
    strcat(line, "-");
  if (att.st_mode & S_IXOTH)
    strcat(line, "x");
  else
    strcat(line, "-");

  printf("%s %ld\t%s %s %ld\t%02d/%02d %02d:%02d ", line, att.st_nlink,
         (*usr).pw_name, (*grp).gr_name, att.st_size, (*time).tm_mday,
         (*time).tm_mon + 1, (*time).tm_hour, (*time).tm_min);
}
int listDir(char *path, int all, int longlist) {
  DIR *dir;
  struct dirent *ent;
  dir = opendir(path);
  if (dir == NULL) {
    printf("Unknown path.\n");
    return 1;
  }
  while ((ent = readdir(dir)) != NULL) {
    if ((*ent).d_name[0] == '.' && !all) continue;
    if (longlist == 1) {
      char filepath[255] = "\0";
      strcat(filepath, path);
      strcat(filepath, "/");
      strcat(filepath, (*ent).d_name);
      // printf("%s\n",filepath);
      displaylong(filepath);
      //free((char*)filepath);
    }
    printf("%s\n", (*ent).d_name);
  }
  closedir(dir);

  return 0;
}

int processCommand(char *args[], int *argv) {
  int status = 1;
  if (strcmp(args[0], "ls") == 0) {
    int a = 0, l = 0, opt;
    char *path = ".\0";
    while ((opt = getopt(*argv, args, "al")) != -1) switch (opt) {
        case 'a':
          a = 1;
          break;
        case 'l':
          l = 1;
          break;
        case '?':
          break;
      }
    if (optind < *argv) {
      path = args[optind];
    }
    optind = 0;
    status = listDir(path, a, l);
    // printf("Options are: %d %d\n", a, l);
    // printf("Path: %s\n", path);

  } else if (strcmp(args[0], "cat") == 0) {
    if (*argv < 2) {
      printf("Missing file path.\n");
      return 1;
    }
    if (*argv > 2) {
      printf("Too many arguments.\n");
      return 1;
    }
    status = printFile(args[1]);
  }
  return status;
}
