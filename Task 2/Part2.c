#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    int a=10, b=25, fq=0, fr=0;
    int child1_exit_code;
    int child2_exit_code;
    fq=fork();// fork a child- Process Q
    
    if(fq==0){//child successfully forked
        a=a+b;
        printf("C1 Value of A: %d B:%d Process_ID: %d \n",a,b,getpid());
        fr= fork(); //fork another child- call it Process R
        if(fr!=0){
           b=b+20;
           printf("C1 Value of A: %d B:%d Process_ID: %d \n",a,b,getpid());
        }
        else{
            a=(a*b)+30;
            printf("C2 Value of A: %d B:%d Process_ID: %d \n",a,b,getpid());
        }
    }
    else{
        
        b=a+b-5;
        printf("P Value of A: %d B:%d Process_ID: %d \n",a,b,getpid());
    }

    return 0;

}

