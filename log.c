#include <time.h>

#include "log.h"
#include <stdlib.h>
#include <stdio.h>

time_t t;

void Log(char *message) {
  FILE *file;

  file = fopen(ACCESS_LOG_FILE, "a");

  if(file == NULL){
    return;
  } else {
    // The file is created let's log and clse the file

    // TODO put it in a function called by other loggers
    t = time(NULL);
    char s[1000];
    struct tm * p = localtime(&t);
    strftime(s, 1000, "%A, %B %d %Y %H:%H:%S", p);
    fprintf(file, "[%s]: %s", s, message);
  }

  if(file){
    fclose(file);
  }
}

void LogErr(char *message) {
  FILE *file;

  // Create access log file if it's not created
  file = fopen(ERROR_LOG_FILE, "a");

  if(file == NULL){
    return;
  } else {
    // The file is created
    fputs(message, file);
  }

  if(file){
    fclose(file);
  }
}

void LogErrExit(char *message) {
  FILE *file;

  file = fopen(ERROR_LOG_FILE, "a");

  if(file == NULL){
    return;
  } else {
    // The file is created
    fputs(message, file);
    exit(EXIT_FAILURE);
  }

  if(file){
    fclose(file);
  }
}
