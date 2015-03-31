#include "cmd_handler.h"

const char * CmdStrings[] = { "USER","PASS","SYST","PORT","RETR","STOR","LIST"};


enum Command_Type getCommandType(char* cmd){
  for (int i = 0;i<NB_CMD;i++){
    int ok = 1;
    for(int j=0 ;j<CMD_LENGTH;j++){
      if(cmd[j]!=CmdStrings[i][j]){
        ok=0;
      }
    }
    if(ok==1)
    return i;
  }
  return UNKNOWN;
}

const char * getStrForCmd( int enumVal )
{
  return CmdStrings[enumVal];
}

//static void handleRequest(int cfd, struct sockaddr_in dist_addr)
void handleRequest(int cfd)
{

  int connectionState = 0;

  char buf[BUFFER_SIZE];
  ssize_t n_bytes_read;
  char cmd[4];

  if(connectionState ==0){
    respond(cfd, 220, "OK");
    connectionState++;
  }


  while ((n_bytes_read = readLine(cfd, buf, BUFFER_SIZE)) > 0) {
    buf[n_bytes_read] = '\0';
    strncpy(cmd,buf,CMD_LENGTH);
    switch(getCommandType(cmd)){
      case USER:
      printf("User command received\n");
      respond(cfd, 331, "Please specify a password");
      break;
      case PASS:
      respond(cfd, 230, "Login successful.");
      printf("Pass command received\n");
      break;
      case SYST:
      respond(cfd, 215, "Unix");
      printf("Syst command received\n");
      break;
      case PORT:
      printf("Port command received\n");
      break;
      case RETR:
      printf("Retr command received\n");
      break;
      case STOR:
      break;
      case LIST:
      break;
      case UNKNOWN:
      break;
    }
  }

}

void respond(int cfd,int code, char* str){
  char response[BUFFER_SIZE];
  sprintf(response,"%d %s\n", code,str);
  printf("Respond: %d %s\n", code,str);
  if (write(cfd, response, strlen(response)) != strlen(response)){
    LogErrExit("write() failed");
  }

}
