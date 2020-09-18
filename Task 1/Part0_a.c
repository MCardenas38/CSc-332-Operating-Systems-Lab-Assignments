#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]){

    //checks the amount of arguments
    if(argc != 2){
        printf("Needs only 1 txt file. \n");
        return -1;
    }

	char* filepath= argv[1];
	int returnval_access;
	int returnval_read;
	int returnval_write;

	//checks if file exists
	returnval_access= access(filepath, F_OK);

	//enters if statement if file exists
	if(returnval_access == 0){
		printf("%s exists\n", filepath);

		//checks if there is read and write access to the file
		returnval_read= access(filepath, R_OK);
		returnval_write= access(filepath, W_OK);

		//prints success or error message of permission
		if(returnval_read == 0)
			printf("Read access to %s \n", filepath);
		else if (returnval_read == -1){
			perror("Read");
            return -1;
        }

		if(returnval_write == 0)
			printf("Write access to %s \n", filepath);
		else if (returnval_write == -1){
			perror("Write");
            return -1;
        }

	}
	else{

		//prints error message
		if(errno == ENOENT)
			printf("%s does not exist\n", filepath);
		else if(errno == EACCES)
			printf("%s is not accessible\n", filepath);
		return -1;
	}

	return 0;
}
