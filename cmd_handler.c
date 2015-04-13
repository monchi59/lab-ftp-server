#include "cmd_handler.h"
#include "filesystem.h"


#define BUF_SIZE 4096   // Buffer size
#define DATA_PORT 55556 // Listening server port number for passive mode

// List of supported commands
const char * CmdStrings[] = { "USER","PASS","SYST","PORT","RETR","STOR","LIST","TYPE", "FEAT","PWD ", "PASV", "QUIT"};

//
void handleRequest(int cfd, struct sockaddr dist_addr){

  // Log the new connexion
  struct sockaddr_in dist_addr_in = *((struct sockaddr_in*)&dist_addr);
  char ipString[BUFFER_SIZE];
  sprintf(ipString,"Connexion from %s", inet_ntoa(dist_addr_in.sin_addr));
  Log(ipString);

  // Initialize current server type mode
  char * type = "A";

  // Declare needed variabless
  char buf[BUFFER_SIZE];
  ssize_t n_bytes_read;
  char cmd[4];

  // First connextion respond.
  int connectionState = 0;
  if(connectionState == 0){
    respond(cfd, 220, "OK");
    connectionState++;
  }

  // Active = 0, Passive = 1
  int mode = 0;
  pid_t data_connexion_pid;

  // data connexion settings
  int data_client_fd; /* Listening and connected sockets */
  int dataPort;
  char fileName[PATH_MAX];
  // For passive mode
  int listening_data_fd; /* Listening and connected sockets */

  // While bits are received from the client.
  while ((n_bytes_read = readLine(cfd, buf, BUFFER_SIZE)) > 0) {
    buf[n_bytes_read] = '\0';
    strncpy(cmd,buf,CMD_LENGTH);
    switch(getCommandType(cmd)){

      // USER command: no check, always respond 331.
      case USER:
      printf("User command received\n");
      respond(cfd, 230, "Login successful.");

      //respond(cfd, 331, "Please specify a password");
      break;

      // PASS command: no check, always respond 230.
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
      openDataConnexion(&data_client_fd, &listening_data_fd, &data_connexion_pid, dist_addr, dataPort);
      respond(cfd,200,"Port command successful.");
      break;
      case RETR:
      printf("Retr command received\n");
      respond(cfd, 150, "Accepted data connection");
      parseFileName(buf,fileName);
      getFileData(data_client_fd,fileName);

	    close(data_client_fd);
      respond(cfd,226,"File successfully transferred.");
      break;

      case STOR:
      printf("Stor command received\n");
      respond(cfd, 150, "Accepted data connection");
      parseFileName(buf,fileName);
      storFile(data_client_fd,fileName);

	    close(data_client_fd);
      respond(cfd,226,"File successfully transferred.");
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
      break;

      case FEAT://Filezilla needs this command, that asks for features of the server, so we send and empty list.
      respond(cfd,211,"Extensions supported:");
      respond(cfd,211,"End");
      break;

      case PWD:
      printf("PWD command received\n");
      char wd[PATH_MAX];
      getWD(wd);
      //sprintf(wd,"%s\"",wd);
      strcat(wd," is your current location");
      respond(cfd,257,wd);
      //257 "/home/parkki" is your current location
      break;

      case PASV:
      printf("PASV command received, entering passive mode\n");
      char resp[BUF_SIZE];
      mode = 1;
      sprintf(resp, "Entering Passive Mode (127,0,0,1,%d,%d)", (DATA_PORT>>8), ((DATA_PORT<<(3*8))>>(3*8)));
      respond(cfd,227,resp);
      openDataConnexion(&data_client_fd, &listening_data_fd, &data_connexion_pid, dist_addr, -1);

      break;

      case QUIT:
      if(mode){
        printf("passive mode QUIT command received\n");
        close(data_client_fd);
        //close(listening_data_fd);
        if(kill(data_connexion_pid, 9) == 0){

        } else {
          printf("erro: %d", errno);
        }
      }
      printf("QUIT command received\n");
      close(cfd);
      break;

      case UNKNOWN:
      printf("Unknown command : %s\n",buf);
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



void openDataConnexion(int * data_client_fd, int * listening_data_fd, pid_t * data_connexion_pid, struct sockaddr client_addr, int port){

  *data_client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (*data_client_fd == -1){
    LogErrExit("data connexion socket");
  }

  struct sockaddr_in client_addr_in = *((struct sockaddr_in*)&client_addr);      /* server's address information */
  if(port != -1){ // If active mode
    client_addr_in.sin_port = htons(port);  /* short, network byte order */


    /* Connect to the server host */
    if (connect(*data_client_fd, (struct sockaddr *)&client_addr_in, sizeof(struct sockaddr)) == -1){
      LogErrExit("Failure connect");
    }
  } else { // passive mode
    struct sockaddr_in server_data_addr; /* this server data address information */
    struct sockaddr client_data_addr; /* client address information */

    initialise_server(listening_data_fd, &server_data_addr,DATA_PORT);

    int connexion = 0;
    while(connexion == 0){
        socklen_t  addrlen = sizeof(struct sockaddr);
        *data_client_fd = accept(*listening_data_fd, &client_data_addr, &addrlen);  /* Wait for connection */
        if (*data_client_fd == -1) {
          LogErrExit("Failure in accept()");
        }

        /* Handle each client request in a new child process */
        switch (fork()) {
        case -1:
            LogErrExit("Fork issue: Can't create child");
            close(*data_client_fd);                 /* Give up on this client */
            break;                      /* May be temporary; try next client */
        case 0:                         /* Child */
            // Handle the request
            *data_connexion_pid = getpid();
            printf("passive mode not yet supported\n");
            // Exit the loop
            close(*listening_data_fd);
            connexion = 1;
            //exit(EXIT_SUCCESS);
            break;

        default:                        /* Parent */
            close(*data_client_fd);                 /* Unneeded copy of connected socket */
            break;                      /* Loop to accept next connection */
        }
    }
  }
}


// Returns the known command, UNKNOWN otherwise
enum Command_Type getCommandType(char* cmd){
  for (int i = 0;i<NB_CMD;i++){
    int ok = 1;
    for(int j=0 ;j<CMD_LENGTH;j++){
      if(cmd[j]!=CmdStrings[i][j]&&CmdStrings[i][j]!=' '){
        ok=0;
      }
    }
    if(ok==1)
    return i;
  }
  return UNKNOWN;
}

// Get the command string from its index.
const char * getStrForCmd(int enumVal)
{
  return CmdStrings[enumVal];
}

// Parse the port from the request string.
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

// Parse a filename from the command string.
void parseFileName(char* buffer,char * fileName){
	strcpy(fileName,buffer+5);
    fileName[strlen(fileName)-2]='\0';
}

// Parse the Type from the command string.
char * getType(char * typeRequest){
  char delimiter[2] = " ";
  char str[BUF_SIZE];
  char * type;
  strcpy(str, typeRequest);

  type = strtok(str, delimiter);
  type = strtok(NULL, delimiter);

  return type;
}
