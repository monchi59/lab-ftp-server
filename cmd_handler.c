#include "cmd_handler.h"

#define BUF_SIZE 4096   // Buffer size

const char * CmdStrings[] = { "USER","PASS","SYST","PORT","RETR","STOR","LIST","FEAT"};


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

int getPort(char * portRequest){

  char delimiter[2] = ",";
  char str[BUF_SIZE];
  char * port1;
  char * port2;
  strcpy(str, portRequest);

  port1 = strtok(str, delimiter);
  port1 = strtok(NULL, delimiter);
  port1 = strtok(NULL, delimiter);
  port1 = strtok(NULL, delimiter);
  port1 = strtok(NULL, delimiter);
  port2 = strtok(NULL, delimiter);

  int port = atoi(port1);
  port = port<<8;
  port = port + atoi(port2);

   return port;
}

//static void handleRequest(int cfd, struct sockaddr_in dist_addr)
void handleRequest(int cfd)
{

  int connectionState = 0;
  char * type = "A";

  char buf[BUFFER_SIZE];
  ssize_t n_bytes_read;
  char cmd[4];

  if(connectionState ==0){
    respond(cfd, 220, "OK");
    connectionState++;
  }

  int data_fd; /* Listening and connected sockets */
  struct sockaddr_in data_addr; /* this server address information */


  while ((n_bytes_read = readLine(cfd, buf, BUFFER_SIZE)) > 0) {
    buf[n_bytes_read] = '\0';
    strncpy(cmd,buf,CMD_LENGTH);
    switch(getCommandType(cmd)){
      case USER:
      printf("User command received\n");
      respond(cfd, 331, "Please specify a password");
      break;
      case PASS:
      printf("Pass command received\n");
      respond(cfd, 230, "Login successful.");
      break;
      case SYST:
      printf("Syst command received\n");
      respond(cfd, 215, "Unix");
      break;
      case PORT:
      int port = getPort(buf);
      printf("Port command received, port: %d\n", );
      initialise_server(&data_fd, &data_addr,port);
      // Create new connexion on new port.
      break;
      case RETR:
      printf("Retr command received\n");
      break;
      case STOR:
      break;
      case LIST:
      respond(cfd, 150, "Here comes the directory list");
      //TODO send the list via the data channel
      break;
      case FEAT://Filezilla needs this command, that asks for features of the server, so we send and empty list.
      respond(cfd,211,"Extensions supported:");
      respond(cfd,211,"End");
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
void sendData(int data_connection_fd,int data_file_fd){
	struct sockaddr_in connected_fd = accept(data_connection_fd, NULL, NULL);  /* Wait for connection */
      if (connected_fd == -1) {
        LogErrExit("Failure in accept()\n");
      }

      /* Handle each client request in a new child process */
      switch (fork()) {
      case -1:
          LogErrExit("Fork issue: Can't create child (%s)");
          close(connected_fd);                 /* Give up on this client */
          break;                      /* May be temporary; try next client */
      case 0:                         /* Child */
          // Handle the request
          handleRequest(connected_fd);
          close(listening_fd);                 /* Unneeded copy of listening socket */
          /* The child process has to call _exit() instead of exit() so that it
          does not flush stdio buffers and that only one process calls exit handlers. */
          exit(EXIT_SUCCESS);

      default:                        /* Parent */
          close(connected_fd);                 /* Unneeded copy of connected socket */
          break;                      /* Loop to accept next connection */
      }
}
