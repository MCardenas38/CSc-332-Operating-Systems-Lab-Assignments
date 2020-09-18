#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){

    //checks the amount of arguments
    if(argc != 2){
        printf("Needs 1 txt file to print. \n");
        return -1;
    }

	char* filepath= argv[1];
	int returnval;

	//checks if the file exists
	returnval= access(filepath, F_OK);

	//file exists
	if(returnval == 0){

        //Opens file
		int fd = open(filepath, O_RDONLY);

		//file open() with read permission fails
		if(fd == -1){
			//file open() with read permission fails
			perror("Open");
			return -1;
		}
        //file sucessfully opened
		else{
			struct stat stats;

			//file properties stored
			stat(filepath,&stats);

			// buffer created
			char data[stats.st_size];
			returnval=read(fd,data,stats.st_size);

			if(returnval<0){
				perror("Read");
                close(fd);
                return -1;
            }
			else{
				//writes content of buffer to terminal
			    write(STDOUT_FILENO, data, stats.st_size);
			}

			//closes open file
			close(fd);
			return 0;

		}

	}
    
    //file does not exists
	else{
		if(errno == ENOENT)
			printf("%s does not exist\n",filepath);
		else if(errno == EACCES)
			printf("%s is not accessible \n", filepath);

		return -1;

	}


}
