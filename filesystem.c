#include "filesystem.h"

void getWD(char* wd){
	getcwd(wd,PATH_MAX);
}

char * insertStr(char * src, char * dst, int index){
	int srcLen = strlen(src);
	if(index < 0 && index > srcLen-1){
		char * result = malloc(srcLen+1);
		strcat(result, src);
		return result;
	}
	int dstLen = strlen(dst);
	char * result = malloc(srcLen + dstLen +1);
	char * tmp = malloc(srcLen-index+1);

	memcpy(result, src, index);
	strcat(result, dst);
	memcpy(tmp, src+index, srcLen-index+1);
	strcat(result, tmp);
	free(tmp);

	return result;
}

char * addBackR(char* str){
	int strLenght = strlen(str);
	char n[] = "\n";
	char r[] = "\r";
	char * result = str;

	char toCompare[2];
	if(strLenght >= 2){
		int i;
		int shift = 0;
		for(i=0; i < strLenght; i++){
			sprintf(toCompare, "%c", str[i]);
			if(strcmp(toCompare, n) == 0){
				result = insertStr(result, r, i+shift);
				shift = shift + strlen(r);
			}
		}
	}
	return result;
}

void listDir(int output_fd,char* directory)
{
	FILE *fp;
	char path[PATH_MAX];
	char cmd[PATH_MAX];
	getWD(path);
	strcat(path,"/");
	strcat(path,directory);

	sprintf(cmd,"ls -l %s\n",path);

	fp = popen(cmd, "r");
	if (fp == NULL)
	    LogErr("Dir list failed");

// char * dirList;
 //char * tmp;



	while (fgets(path, PATH_MAX, fp) != NULL)
			write(output_fd, addBackR(path), strlen(addBackR(path)));

	pclose(fp);
}

void getFileData(int output_fd,char* file){

	char path[PATH_MAX];
	getWD(path);
	strcat(path,"/");
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
	strcat(path,"/");
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
