#ifndef CMD_HANDLER_H
#define CMD_HANDLER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include "read_line.h"

#define NB_CMD 7
#define CMD_LENGTH 4
#define BUFFER_SIZE 128

enum Command_Type{ USER, PASS, SYST, PORT, RETR, STOR, LIST,UNKNOWN};

const char * CmdStrings[] = { "USER","PASS","SYST","PORT","RETR","STOR","LIST"};
const char * getStrForCmd( int enumVal );
enum Command_Type getCommandType(char* cmd);

static void handleRequest(int cfd, struct sockaddr_in dist_addr);

void respond(int cfd,int code, char* str);

#endif