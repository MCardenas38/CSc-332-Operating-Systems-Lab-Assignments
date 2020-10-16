#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]){

    //creating childern	
    pid_t child1;
    pid_t child2;

    //prints error if forking child1 fails
    if((child1=fork())==-1){
        printf("Failed to fork first child!\n");
        return -1;   
    }
    
    if(child1==0){//child1 process

        //arguments for execv
        char *args_child1[]= {"./Process_P1",NULL};

        //executes Process_P1.c
        execv(args_child1[0],args_child1);
    }
    else{

        //prints error if forking child2 fails
        if((child2=fork())==-1){
            printf("Failed to fork first child!\n");
            return -1;   
        }

        if(child2==0){//child2 process

            //puts child2 process to sleep for a second to allow the files to be created n child1 process
            sleep(1);
        
            //arguments for execv
            char *args_child2[]= {"./Process_P2",NULL};

            //executes Process_P2.c
            execv(args_child2[0],args_child2);
        }
        else{//parent process
            int status_child1;
            int status_child2;

            //Parent process waits for childern process to end
            waitpid(child1,&status_child1,0);
            waitpid(child2,&status_child2,0);
            printf("Done!\n");
        }

    }

	return 0;
}
