#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>

//creating mutex locks
pthread_mutex_t lock= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smoker_match= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smoker_paper= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smoker_tobacco= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t agent= PTHREAD_MUTEX_INITIALIZER;

void *process_agent(){

     int rand_num;

     for(int i =0;i<10;++i){
        pthread_mutex_lock(&lock);
        rand_num= (rand() % 3) + 1;
        if(rand_num==1){
            printf("Paper and Tobacco placed on table...\n");
            pthread_mutex_unlock(&smoker_match);
        }
        else if(rand_num==2){
            printf("Match and Tobacco placed on table...\n");
            pthread_mutex_unlock(&smoker_paper);
        }
        else if(rand_num==3){
            printf("Paper and Match placed on table...\n");
            pthread_mutex_unlock(&smoker_tobacco);
        }
        pthread_mutex_unlock(&lock);
        pthread_mutex_lock(&agent);
     }

}

void *process_smoker_match(){
    while(1){
        pthread_mutex_lock(&smoker_match);
        pthread_mutex_lock (&lock);
        printf("Smoker with match picks up paper and tobacco...\n");
        printf("Smoking....\n\n");
        pthread_mutex_unlock(&agent);
        pthread_mutex_unlock(&lock);
    }
}

void *process_smoker_paper(){
    while(1){
        pthread_mutex_lock(&smoker_paper);
        pthread_mutex_lock (&lock);
        printf("Smoker with paper picks up match and tobacco...\n");
        printf("Smoking....\n\n");
        pthread_mutex_unlock(&agent);
        pthread_mutex_unlock(&lock);
    }
}

void *process_smoker_tobacco(){
    while(1){
        pthread_mutex_lock(&smoker_tobacco);
        pthread_mutex_lock (&lock);
        printf("Smoker with tobacco picks up paper and match...\n");
        printf("Smoking....\n\n");
        pthread_mutex_unlock(&agent);
        pthread_mutex_unlock(&lock);
    }
}

int main(){

    //seed
    srand(time(0));

    //thread array
    pthread_t tid[4];


    //initializing mutex locks
    if(pthread_mutex_init(&lock, NULL) != 0){
        printf("Mutex init failed \n");
        return 1;
    }
    if(pthread_mutex_init(&smoker_match, NULL) != 0){
        printf("Mutex init failed \n");
        return 1;
    }
    if(pthread_mutex_init(&smoker_paper, NULL) != 0){
        printf("Mutex init failed \n");
        return 1;
    }
    if(pthread_mutex_init(&smoker_tobacco, NULL) != 0){
        printf("Mutex init failed \n");
        return 1;
    }
    if(pthread_mutex_init(&agent, NULL) != 0){
        printf("Mutex init failed \n");
        return 1;
    }

    //locks all mutexs except for lock
    pthread_mutex_lock(&smoker_match);
    pthread_mutex_lock(&smoker_paper);
    pthread_mutex_lock(&smoker_tobacco);
    pthread_mutex_lock(&agent);

    //assigning each thread to a function
    pthread_create(&tid[0],NULL,process_agent,NULL);
    pthread_create(&tid[1],NULL,process_smoker_match,NULL);
    pthread_create(&tid[2],NULL,process_smoker_paper,NULL);
    pthread_create(&tid[3],NULL,process_smoker_tobacco,NULL);

    //waiting for agent to end to terminate program
    pthread_join(tid[0],NULL); 

    return 0;
}


