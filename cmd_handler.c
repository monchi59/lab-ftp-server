#include "cmd_handler.h"
#include "filesystem.h"


#define BUF_SIZE 4096   // Buffer size
#define DATA_PORT 55554 // Listening server port number

const char * CmdStrings[] = { "USER","PASS","SYST","PORT","RETR","STOR","LIST","TYPE", "FEAT"};

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

const char * getStrForCmd(int enumVal)
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
  int i;
  for(i = 0; i < 4; i++){
    port1 = strtok(NULL, delimiter);
  }
  port2 = strtok(NULL, delimiter);

  int port = atoi(port1);
  port = port<<8;
  port = port + atoi(port2);

  return port;
}

void parseFileName(char* buffer,char * cmdString,char * fileName){
	fileName = buffer + 4;
}

char * getType(char * typeRequest){
  char delimiter[2] = " ";
  char str[BUF_SIZE];
  char * type;
  strcpy(str, typeRequest);

  type = strtok(str, delimiter);
  type = strtok(NULL, delimiter);

  return type;
}

void handleRequest(int cfd, struct sockaddr dist_addr){

  // Log the new connexion
  struct sockaddr_in dist_addr_in = *((struct sockaddr_in*)&dist_addr);
  char ipString[BUFFER_SIZE];
  sprintf(ipString,"Connexion from %s", inet_ntoa(dist_addr_in.sin_addr));
  Log(ipString);

  // Initialize current server type mode
  char * type = "A";

  char buf[BUFFER_SIZE];
  ssize_t n_bytes_read;
  char cmd[4];

  int connectionState = 0;

  if(connectionState == 0){
    respond(cfd, 220, "OK");
    connectionState++;
  }

  // data connexion settings
  int data_client_fd; /* Listening and connected sockets */
  int dataPort;


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
      respond(cfd, 215, "UNIX Type: L8");
      break;
      case PORT:
      dataPort = getPort(buf);
      printf("Port command received, port: %d\n", dataPort);
      openDataConnexion(&data_client_fd, dist_addr, dataPort);
      respond(cfd,200,"Port command successful.");
      break;
      case RETR:
      printf("Retr command received\n");
      break;
      case STOR:
      break;
      case LIST:
      printf("List command received\n");
      respond(cfd, 150, "Here comes the directory list");

      listDir(data_client_fd, ".");

      close(data_client_fd);
      respond(cfd,226,"Directory send OK.");
      break;
      case TYPE:
      printf("Type cmd\n");
      type = getType(buf);
      printf("Type command received, type: %s\n", type);
      respond(cfd, 200, "Switching to type");
      case FEAT://Filezilla needs this command, that asks for features of the server, so we send and empty list.
      respond(cfd,211,"Extensions supported:");
      respond(cfd,211,"End");
      break;
      case UNKNOWN:
      printf("Unknown command\n");
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


void respondData(int data_connected_fd, char* str){
  char response[BUFFER_SIZE];
  sprintf(response,"%s\n", str);
  printf("Respond data: %s\n",str);
  if (write(data_connected_fd, response, strlen(response)) != strlen(response)){
    LogErrExit("write() failed");
  }

}



void openDataConnexion(int * data_client_fd, struct sockaddr client_addr, int port){

  *data_client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (*data_client_fd == -1){
    LogErrExit("data connexion socket");
  }

  struct sockaddr_in client_addr_in = *((struct sockaddr_in*)&client_addr);      /* server's address information */
  client_addr_in.sin_port = htons(port);  /* short, network byte order */

  /* Connect to the server host */
  if (connect(*data_client_fd, (struct sockaddr *)&client_addr_in, sizeof(struct sockaddr)) == -1){
    LogErrExit("Failure connect");
  }
}
