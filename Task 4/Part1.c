#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

    //input buffer
    char input[100];
    
    //compares current input for quit
    while(strcmp("quit",input) != 0){

        pid_t interpreter;
        int int_exit_code;

        char *p;

        printf("Enter command: ");
        //gets whole input
        fgets(input,sizeof(input),stdin);

        //removes the new line char from string
        if((p=strchr(input,'\n'))!=NULL){
                *p= '\0';            
        }

        char * token= strtok(input," ");
        char * arg[100]={};
        int counter=0;
        
        //stores each command into arg array
        while(token != NULL){
            arg[counter]= token;
            token= strtok(NULL," ");
            counter++;        
        }

        //sets last element in arg array to null
        arg[counter] = NULL;

        //forks child
        if((interpreter=fork())==-1){
            printf("Error forking child.\n");
            return -1;
        }
        
        if(interpreter==0){
            //executes commands
            execvp(arg[0],arg);
            exit(0);
        }
        else{
            //parent process waits for child to end
            waitpid(interpreter,&int_exit_code,0);
        }

    }

    return 0;
}

