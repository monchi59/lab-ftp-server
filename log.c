#include <time.h>

#include "log.h"
#include <stdlib.h>
#include <stdio.h>

time_t t;

void addTime(char * message){
  t = time(NULL);
  char s[1000];
  char out[1000];
  struct tm * p = localtime(&t);
  strftime(s, 1000, "%F %X", p);
  sprintf(out, "[%s]: %s", s, message);
  strcpy(message, out);
}

void Log(char *message) {
  FILE *file;

  file = fopen(ACCESS_LOG_FILE, "a");

  if(file == NULL){
    return;
  } else {
    // The file is created let's log and clse the file

    addTime(message);
    fprintf(file, "%s\n", message);
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
    addTime(message);
    fprintf(file, "%s\n", message);
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
    addTime(message);
    fprintf(file, "%s\n", message);
    exit(EXIT_FAILURE);
  }

  if(file){
    fclose(file);
  }
}
