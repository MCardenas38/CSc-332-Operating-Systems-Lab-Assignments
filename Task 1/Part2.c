#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){

    //checks the amount of arguments
    if(argc > 3 || argc==1){
        printf("Needs 2 txt files. \n");
        return -1;
    }

	char* filepath= argv[1];
	int returnval;

	//checks if file exists
	returnval= access(filepath, F_OK);

    //enters if statement if file exists
	if(returnval == 0){

		//opens first file with read only permission
		int fd1 = open(filepath, O_RDONLY);

		if(fd1 == -1){
			//displays error if open() fails
			perror("Open");
			return -1;
		}
		else{
			struct stat stats;

			//used to get first file data
			stat(filepath,&stats);

			//creates buffer with the number of bytes in first file
			char data[stats.st_size];

			// reads file and stores information to the data buffer
			returnval=read(fd1,data,stats.st_size);

			if(returnval<0){
				//displays error message if read() fails
				perror("Read");
                return -1;
            }
			else{
				filepath= argv[2];
				int fd2;

                //if second file exists open with write permission
				if(access(filepath, F_OK) == 0){
					fd2= open(filepath, O_WRONLY);

                    //if open() fails for second file display error message
                    if(fd2<0){
                        perror("Open");
                        
                        //closes files
                        close(fd1);
                        close(fd2);
					    return -1;
                    }
				}

				//writes information from data buffer to second file
				write(fd2,data,stats.st_size);

                printf("File copy successful. \n"); 

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

		return -1;

	}
}
