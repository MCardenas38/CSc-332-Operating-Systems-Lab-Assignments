#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

    FILE *fp;
    int x=2;
    int y=2;
    int col= x*y;

    //opens files
    fp= fopen("input.txt", "r");

    //2D array to store numbers
    int num[10][col];

    //storing numbers in file
    for(int i=0; i<10; ++i){
        for(int j=0; j<col; ++j){
            //scans each number and turns it an int, then stores it in the 2d array
            int data;
            fscanf(fp,"%d",&data);
            num[i][j]= data;
        }
    }

    //forloop to fork x number (managers) of processes
    for(int i=0;i<x;++i){
        pid_t pid1;
        if((pid1=fork())==-1){
            return -1;
        } 
        
        //manager process
        if(pid1==0){

            //forloop to fork y number (workers) of processes
            for(int j=0;j<y;++j){
                pid_t pid2;
                if((pid2=fork())==-1){
                    return -1;
                } 
                //worker process
                if(pid2==0){
                    float average=0;
                    int max=0;
                    for(int s=0;s<10;++s){
                        int result= num[s][(i*2)+j];
                        average += result;
                        if(result>max){
                            max= result;
                        }
                    }

                    printf("[P-%d][C-%d] Chapter %d: Homework %d Average: %f | Max: %d \n",getppid(),getpid(),i+1,j+1,average/10,max);
    
                    exit(0);

                }
            }
            
            //manager waits for workers to finish
            for(int j=0;j<y;++j){
                wait(NULL);
            }
            
            exit(0);
        }
    }

    //main process waits for manager to finish
    for(int i=0;i<x;++i){
        wait(NULL);
    }

    //closes file
    fclose(fp);

    return 0;
}

