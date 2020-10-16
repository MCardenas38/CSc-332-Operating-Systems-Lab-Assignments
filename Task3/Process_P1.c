#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]){
	//Creates destination text file with read and write permissions
	int fd1= open("destination1.txt", O_CREAT,00700);
    int fd2= open("destination2.txt", O_CREAT,00700);

    if(fd1<0){
        perror("destination1.txt");
    }
    if(fd2<0){
        perror("destination2.txt");
    }
    
    close(fd1);
    close(fd2);

	return 0;
}
