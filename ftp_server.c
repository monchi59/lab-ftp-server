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
#include "cmd_handler.h"

#define PORT 55555      // Listening server port number
#define BUF_SIZE 4096   // Buffer size
#define BACKLOG 10      // How many pending connections queue will hold

#define LOGFILE error.log

int main(int argc, char *argv[])
{

  int listening_fd, connected_fd; /* Listening and connected sockets */
  struct sockaddr_in server_addr; /* this server address information */
  struct sockaddr client_addr; /* client address information */

  initialise_server(&listening_fd, &server_addr,PORT);

  printf("FTP server listening on port: %d\n", ntohs(server_addr.sin_port));

  // Infinit loop to listen requests
  for (;;) {
      socklen_t  addrlen = sizeof(struct sockaddr);
      connected_fd = accept(listening_fd, &client_addr, &addrlen);  /* Wait for connection */
      if (connected_fd == -1) {
        LogErrExit("Failure in accept()");
      }

      /* Handle each client request in a new child process */
      switch (fork()) {
      case -1:
          LogErrExit("Fork issue: Can't create child");
          close(connected_fd);                 /* Give up on this client */
          break;                      /* May be temporary; try next client */
      case 0:                         /* Child */
          // Handle the request
          handleRequest(connected_fd, client_addr);
          close(listening_fd);                 /* Unneeded copy of listening socket */
          /* The child process has to call _exit() instead of exit() so that it
          does not flush stdio buffers and that only one process calls exit handlers. */
          exit(EXIT_SUCCESS);

      default:                        /* Parent */
          close(connected_fd);                 /* Unneeded copy of connected socket */
          break;                      /* Loop to accept next connection */
      }
  }

  return EXIT_SUCCESS;

}

void initialise_server(int * listening_fd, struct sockaddr_in * server_addr,int port){

  // Create the socket file descriptor
  if ((*listening_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    // Exit error case :
    LogErrExit("Socket Creation");
  }

  /* Set the SO_REUSEADDR socket option in order to avoid the EADDRINUSE (Address already in use)
     error when a TCP server is restarted. */
  int optval = 1;
  if (setsockopt(*listening_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1){
    LogErrExit("Set socket Option");
  }

  // Server address initialisation
  server_addr->sin_family = AF_INET;
  server_addr->sin_port = htons(port);     /* short, network byte order */
  server_addr->sin_addr.s_addr = INADDR_ANY; /* automatically fill with my IP */
  memset(&(server_addr->sin_zero), '\0', 8); /* zero the rest of the struct */

  // Binding the server address to the listening socket
  if (bind(*listening_fd, (struct sockaddr *)server_addr, sizeof(struct sockaddr)) == -1){
    printf("Binding error: is the port already in use?\n");
    LogErrExit("Binding");
  }

  /* Marks the socket as a passive socket (listening socket)
     and set the maximum number of pending connections to BACKLOG */
  if (listen(*listening_fd, BACKLOG) == -1){
    LogErrExit("Listening");
  }
}
