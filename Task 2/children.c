#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    pid_t child_1= fork();//fork a child
    int child1_exit_code;
    int child2_exit_code;

    if(child_1==0){//first child process
        printf("I am child one, my pid is: %d \n", getpid());
    }
    else{//parent process
        pid_t child_2= fork();//fork a second child
        if(child_2==0){//second child process
            printf("I am child two, my pid is: %d \n", getpid());
        }
        else{
            waitpid(child_1,&child1_exit_code,0);
            waitpid(child_2,&child2_exit_code,0);
        }
    }

    return 0;
}
