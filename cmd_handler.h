#ifndef CMD_HANDLER_H
#define CMD_HANDLER_H

#define _POSIX_SOURCE

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/limits.h>

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "read_line.h"
#include "log.h"
#include "ftp_server.h"

#define NB_CMD 12
#define CMD_LENGTH 4
#define BUFFER_SIZE 128

enum Command_Type{ USER, PASS, SYST, PORT, RETR, STOR, LIST,TYPE,FEAT,PWD,PASV,QUIT,UNKNOWN};

const char * getStrForCmd( int enumVal );
enum Command_Type getCommandType(char* cmd);
int getPort(char * portRequest);
char * getType(char * typeRequest);

void handleRequest(int cfd, struct sockaddr dist_addr);

void respond(int cfd,int code, char* str);
void respondData(int cfd, char* str);

void openDataConnexion(int * data_listening_fd, int * listening_data_fd, pid_t * data_connexion_pid, struct sockaddr client_addr, int port);

void parseFileName(char* buffer,char * fileName);
#endif
