#ifndef CMD_HANDLER_H
#define CMD_HANDLER_H


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include "read_line.h"
#include "log.h"
#include "ftp_server.h"

#define NB_CMD 9
#define CMD_LENGTH 4
#define BUFFER_SIZE 128

enum Command_Type{ USER, PASS, SYST, PORT, RETR, STOR, LIST,TYPE,FEAT,UNKNOWN};

const char * getStrForCmd( int enumVal );
enum Command_Type getCommandType(char* cmd);
int getPort(char * portRequest);
char * getType(char * typeRequest);

//static void handleRequest(int cfd, struct sockaddr_in dist_addr);
void handleRequest(int cfd);

void respond(int cfd,int code, char* str);
void respondData(int cfd, char* str);

void openDataConnexion(int * data_listening_fd, int * data_connected_fd);

#endif
