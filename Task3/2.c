#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    pid_t child;
    int c1_exit_code;
    
    //error checking
    if((child=fork())==-1){
        printf("Error creating child.");
        return 1;    
    }

    if(child==0){//child
        //Prints PID of child
        printf("Child's PID is %d \n",getpid());

        char* arg[]={"ls","-al",NULL};

        //executes command
        execvp(arg[0],arg);
        
        //prints if execl fails
        printf("Error showing files. \n");
    }
    else{//parent
        int status;
        //waits for child
        waitpid(child,&status,0);
    }

    return 0;
}
