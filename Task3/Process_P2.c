#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]){

    //checks if the file exists
    int returnval= access("source.txt", F_OK);
    int returnval1= access("destination1.txt", F_OK);
    int returnval2= access("destination2.txt", F_OK);
    if(returnval!=0 || returnval1!=0 || returnval2!=0){
        if(returnval<0){
            perror("source.txt");
        }
        else if(returnval1<0){
            perror("destination1.txt");
        }
        else if(returnval2<0){
            perror("destination2.txt");
        }
        
		return -1; 
    }
    else{ 

        //Opens file
		int fd = open("source.txt", O_RDONLY);
        int fd1 = open("destination1.txt", O_WRONLY);
        int fd2 = open("destination2.txt", O_WRONLY);

		if(fd == -1 || fd1==-1 || fd2==-1){
			//file open() with read permission fails
            if(fd<0){
                perror("source.txt");
            }
            else if(fd1<0){
                perror("destination1.txt");
            }
            else if(fd2<0){
                perror("destination2.txt");
            }
			return -1;
		}
		else{//file sucessfully opened
			struct stat stats;

			//file properties stored
			stat("source.txt",&stats);

			// buffer created
			char data_a[50];
            char write_buf[50];
            char data_b[100];
            int counter;
            int read_into;

            //reads through whole file and writes to destination files
            while((lseek(fd,0,SEEK_CUR))!= stats.st_size){

                read_into=read(fd,data_a,50);
                counter=0;
                for(int i=0;i<read_into;++i){
                    if(data_a[i]=='5'||data_a[i]=='8'){
                           write_buf[counter]= data_a[i];
                           ++counter;
                    }
                }
                write(fd1, write_buf, counter);

                read_into=read(fd,data_b,100);
                write(fd2, data_b, read_into);

            }


			//closes open file
			close(fd);
            close(fd1);
            close(fd2);
			return 0;

		}
    }
	

    //remeber to close files
    //close(fd);


	return 0;
}
