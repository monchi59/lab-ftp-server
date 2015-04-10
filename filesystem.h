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
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "log.h"

#define WORKING_DIR "/ftp/"


#define DATA_BUFFER_SIZE 1448

void listDir(int output_fd,char* directory);
void getFileData(int output_fd,char* file);
void storFile(int input_fd,char* file);
void getWD(char* wd);
char * insertStr(char * src, char * dst, int index);
char * addBackR(char* str);


#endif
