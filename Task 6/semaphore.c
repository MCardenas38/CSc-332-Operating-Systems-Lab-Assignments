#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include "sem.h"

int main(){

    //seeds
    srand(time(0));

    int lock;
    int smoker_match;
    int smoker_paper;
    int smoker_tobacco;
    int agent;
    
    //creating semaphore
    if((lock= semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1){
            printf("Create Semaphore Failed!");
            exit(1);
    }

    if((smoker_match= semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1){
            printf("Create Semaphore Failed!");
            exit(1);
    }

    if((smoker_paper= semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1){
            printf("Create Semaphore Failed!");
            exit(1);
    }
        
    if((smoker_tobacco= semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1){
            printf("Create Semaphore Failed!");
            exit(1);
    }

    if((agent= semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1){
            printf("Create Semaphore Failed!");
            exit(1);
    }

    //initializing all semaphores as locked execept for lock
    sem_create(lock, 1);
    sem_create(smoker_match, 0);
    sem_create(smoker_paper, 0);
    sem_create(smoker_tobacco, 0);
    sem_create(agent, 0);


    int sm;
    int sp;
    int st;

    if ((sm = fork()) == -1) {
		//fork failed!
		perror("fork failed!\n");
		exit(1);
	}
    
    if(sm == 0){//smoker_match child process
        while(1){
            P(smoker_match);
            P(lock);
            printf("Smoker with match picks up paper and tobacco...\n");
            printf("Smoking....\n\n");
            V(agent);
            V(lock);
        }
    }
    else{
        if ((sp = fork()) == -1) {
		    //fork failed!
		    perror("fork failed!\n");
		    exit(1);
	    }

        if(sp == 0){//smoker_paper child process
            while(1){
                P(smoker_paper);
                P(lock);
                printf("Smoker with paper picks up match and tobacco...\n");
                printf("Smoking....\n\n");
                V(agent);
                V(lock);
            }

        }
        else{
            if ((st = fork()) == -1) {
	            //fork failed!
	            perror("fork failed!\n");
	            exit(1);
            }

            if(st == 0){//smoker_tobacco child process
                while(1){
                    P(smoker_tobacco);
                    P(lock);
                    printf("Smoker with tobacco picks up paper and match...\n");
                    printf("Smoking....\n\n");
                    V(agent);
                    V(lock);
                }
            }
            else{//agent process (Parent)
                
                 int rand_num;

                 for(int i =0;i<10;++i){
                    P(lock);
                    rand_num= (rand() % 3) + 1;
                    if(rand_num==1){
                        printf("Paper and Tobacco placed on table...\n");
                        V(smoker_match);
                    }
                    else if(rand_num==2){
                        printf("Match and Tobacco placed on table...\n");
                        V(smoker_paper);
                    }
                    else if(rand_num==3){
                        printf("Paper and Match placed on table...\n");
                        V(smoker_tobacco);
                    }
                    V(lock);
                    P(agent);
                 }

                 //kills all childern processes
                 kill(sm,SIGKILL);
                 kill(sp,SIGKILL);
                 kill(st,SIGKILL);

                 exit(0);
            }

        }

    }

    return 0;
}
