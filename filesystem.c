#include "filesystem.h"

void getWD(char* wd){
	getcwd(wd,PATH_MAX);
	strcat(wd,WORKING_DIR);
}

void listDir(int output_fd,char* directory)
{
	FILE *fp;
	char path[PATH_MAX];
	char cmd[PATH_MAX];
	getWD(path);
	strcat(path,directory);

	sprintf(cmd,"ls -l %s",path);

	fp = popen(cmd, "r");
	if (fp == NULL)
	    LogErr("Dir list failed");

	while (fgets(path, PATH_MAX, fp) != NULL)
		write(output_fd, path,strlen(path));

	pclose(fp);
}

void getFileData(int output_fd,char* file){

	char path[PATH_MAX];
	getWD(path);
	strcat(path,file);

	int inputFd = open(path, O_RDONLY);
    if (inputFd == -1){
        LogErr("error opening file");
    }

    int bufferSize = DATA_BUFFER_SIZE;
    char buf[DATA_BUFFER_SIZE];

	while(bufferSize==DATA_BUFFER_SIZE){
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
	char buf[DATA_BUFFER_SIZE];
	char path[PATH_MAX];
	getWD(path);
	strcat(path,file);

	int fileFd = open(path, O_CREAT | O_WRONLY | O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (fileFd == -1){
        LogErr("error opening file");
    }

    int bufferSize = DATA_BUFFER_SIZE;

	while(bufferSize==DATA_BUFFER_SIZE){
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
