#include "filesystem.h"



void listDir(int output_fd,char* directory)
{
	FILE *fp;
	char path[PATH_MAX];
	char cmd[PATH_MAX];

	sprintf(cmd,"ls -l %s",directory);

	fp = popen(cmd, "r");
	if (fp == NULL)
	    LogErr("Dir list failed");

	while (fgets(path, PATH_MAX, fp) != NULL)
	    fprintf((FILE*)&output_fd,"%s", path);


	pclose(fp);
}

void getFileData(int output_fd,char* file){

	int inputFd = open(file, O_RDONLY);
    if (inputFd == -1){
        LogErr("error opening file");
    }

    int bufferSize = BUF_SIZE;
    char buf[BUF_SIZE];

	while(bufferSize==BUF_SIZE){
        bufferSize = read(inputFd,buf,bufferSize);
        if(bufferSize == -1){
            LogErr("read failed");
        }
        if(write(output_fd,buf,bufferSize)== -1){
            LogErr("write failed");
        }
    }

}

void storFile(int input_fd,char* file){
	char buf[BUF_SIZE];

	int fileFd = open(file, O_CREAT | O_WRONLY | O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (fileFd == -1){
        LogErr("error opening file");
    }

    int bufferSize = BUF_SIZE;

	while(bufferSize==BUF_SIZE){
        bufferSize = read(input_fd,buf,bufferSize);
        if(bufferSize == -1){
            LogErr("read failed");
        }
        if(write(fileFd,buf,bufferSize)== -1){
            LogErr("write failed");
        }
    }
    close(fileFd);
}

