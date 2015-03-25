#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ftp_server.h"
#include "log.h"

#define PORT 55555      // Listening server port number
#define BUF_SIZE 4096   // Buffer size
#define BACKLOG 10      // How many pending connections queue will hold

#define LOGFILE error.log

int main(int argc, char *argv[])
{

  int listening_fd, connected_fd; /* Listening and connected sockets */
  struct sockaddr_in server_addr; /* this server address information */

  // Log examples
  Log("Hello accress log\n");
  LogErr("Hello non-blocking error log\n");

  // Create the socket file descriptor
  if ((listening_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    // Exit error case :
    LogErrExit("Socket Creation\n");
  }

  hello();
  return EXIT_SUCCESS;

}

void hello(){

  printf("Hello World\n");
}
