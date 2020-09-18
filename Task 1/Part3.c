#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){

    //checks the amount of arguments
    if(argc != 1){
        printf("Only source.txt and destination.txt files needed in current folder (No arguments needed).\n");
        return -1;
    }

	char* filepath= "source.txt";
	int returnval;

	//checks if file exists
	returnval= access(filepath, F_OK);

	if(returnval == 0){
		//opens first file with read only permission
		int fd1 = open(filepath, O_RDONLY);

		if(fd1 < 0){
			//displays error if open() fails
            perror("Open");
            close(fd1);
			return -1;
		}
		else{
			struct stat stats;

			//used to get first file data
			stat(filepath,&stats);

			filepath= "destination.txt";
			int fd2;

            //checks if second file exists
			if(access(filepath, F_OK) == 0){
				//if second file exists open with write permission
				fd2= open(filepath, O_WRONLY);
			}
			else{
				//if file does not exists create file with read/write permissions
				//fd2= open(filepath,O_RDWR|O_CREAT);
                //prints error if destination.txt does not exist
                perror("destination.txt");
                close(fd1);
                return -1;
			}

            //if open() fails for second file display error message
			if(fd2<0){
                perror("Open");
				//closes files
				close(fd1);
                close(fd2);
				return -1;
			}
			else{
                int buff_size= 75;
                int end_byte= stats.st_size;
                char add[3]= {'A','B','C'};
                char data[buff_size];


                for(int current_byte=0; current_byte != end_byte; current_byte= lseek(fd1,0,SEEK_CUR)){
                        //printf("current byte: %d \n",current_byte);
                        if(current_byte+75 >= end_byte){
                            buff_size= end_byte - current_byte;
                        }
                        //printf("buffer size: %d \n",buff_size);
                        char data[buff_size];
                        read(fd1,data,buff_size);
                        for(int i=0;i<buff_size;++i){
                            if(data[i]=='5')
                                data[i]= 'O';
                        }
			            //writes information from data buffer to second file
			            write(fd2,data,buff_size);
                        write(fd2,add,3);
                }
				//closes both files
				close(fd1);
				close(fd2);
				return 0;
			}
		}

	}
	else{
		//displays error message if first file does not exist
		if(errno == ENOENT)
			printf("%s does not exist\n",filepath);
		else if(errno == EACCES)
			printf("%s is not accessible \n", filepath);

		return 1;

	}
}
