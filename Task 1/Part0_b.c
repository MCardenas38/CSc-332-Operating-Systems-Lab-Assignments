#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]){

    //checks the amount of arguments
    if(argc!=1){
        printf("Needs no files. \n");
        return -1;
    }

	int fd;

	//Creates destination text file with read and write permissions
	fd= open("destination.txt", O_RDWR|O_CREAT,0666);

	//fd returns -1 if open() fails with error message
	if(fd < 0){
		perror("Open");
		return -1;
	}
	//fd returns file descriptor if open successful
	else{
		printf("Created and Opened destination.txt. \n");
		int code;

		//closes open file
		code=close(fd);
		if(code==0)
			printf("File closed \n");
		else{
			perror("Close");
            return -1;
        }
	}


	return 0;
}
