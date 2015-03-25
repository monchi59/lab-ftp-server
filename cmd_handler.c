#include "cmd_handler.h"  

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

static void handleRequest(int cfd, struct sockaddr_in dist_addr)
{  
	char buf[BUFFER_SIZE];
    ssize_t n_bytes_read;
    char cmd[4];


    
	while ((n_bytes_read = readLine(cfd, buf, BUFFER_SIZE)) > 0) {
        buf[n_bytes_read] = '\0';
        strncpy(cmd,buf,CMD_LENGTH);
        switch(getCommandType(cmd)){
        	case USER:
        		break;
        	case PASS:
        		break;
        	case SYST:
        		break;
        	case PORT:
        		break;
        	case RETR:
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
	sprintf(response,"%d %s", code,str);
    if (write(cfd, response, strlen(response)) != strlen(response)){
    	 //errExit("write() failed");
    }
               

}