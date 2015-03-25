#ifndef LOG_H
#define LOG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define ACCESS_LOG_FILE "access.log"
#define ERROR_LOG_FILE "error.log"

bool accressFileCreated; // keeps track whether the log file is created or not
bool errorFileCreated; // keeps track whether the log file is created or not

void Log(char *message); // logs a message to ACCESS_LOG_FILE
void LogErr(char *message); // logs a message to ERROR_LOG_FILE
void LogErrExit(char *message); // logs a message to ERROR_LOG_FILE; execution is interrupted

#endif
