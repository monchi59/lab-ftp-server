#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define _BSD_SOURCE

#include <dirent.h> 
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>
#include <fcntl.h>
#include "log.h"


#define BUF_SIZE 1448

void listDir(int output_fd,char* directory);
void getFileData(int output_fd,char* file);
void storFile(int input_fd,char* file);


#endif
