#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "ftp_server.h"

int main(int argc, char *argv[])
{

    hello();

    return EXIT_SUCCESS;
}

void hello(){

    printf("Hello World\n");

}
