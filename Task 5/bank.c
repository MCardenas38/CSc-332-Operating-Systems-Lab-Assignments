#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "sem.h"

#define CHILD      			0  			/* Return value of child proc from fork call */
#define TRUE       			0  
#define FALSE      			1

FILE *fp1, *fp2, *fp3, *fp4, *fp5, *fp6, *fp7, *fp8;			/* File Pointers */

int main()
{
	int pid;						// Process ID after fork call
	int i;							// Loop index
	int N;							// Number of times dad does update
	int N_Att;						// Number of time sons allowed to do update
	int status;						// Exit status of child process
	int bal1, bal2;					// Balance read by processes
	int flag, flag1;				// End of loop variables

    int dad_counter;                //Dad T(P) Counter
    int son1_counter;               //Son 1 T(P) Counter
    int son2_counter;               //Son 2 T(P) Counter
    int dad_ask;

    int sem;

    if((sem= semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1){
        printf("Create Semaphore Failed!");
        exit(1);
    }

    sem_create(sem, 1);

	
	//Initialize the file balance to be $100
	fp1 = fopen("balance","w");
	bal1 = 100;
	fprintf(fp1, "%d\n", bal1);
	fclose(fp1);
	
	//Initialize the number of attempts to be 20
	fp4 = fopen("attempt", "w");
	N_Att = 20;
	fprintf(fp4, "%d\n", N_Att);
	fclose(fp4);

    //Initialize dad's counter
	fp5 = fopen("dad", "w");
	dad_counter = 0;
	fprintf(fp5, "%d\n", dad_counter);
	fclose(fp5);

    //Initialize son's 1 counter
	fp6 = fopen("son_1", "w");
	son1_counter = 0;
	fprintf(fp6, "%d\n", son1_counter);
	fclose(fp6);

    //Initialize son's 2 counter
	fp7 = fopen("son_2", "w");
	son2_counter = 0;
	fprintf(fp7, "%d\n", son2_counter);
	fclose(fp7);

    //Initialize file to record N 
    fp8 = fopen("dad_ask", "w");
    dad_ask = 0;
    fprintf(fp8, "%d\n", dad_ask);
    fclose(fp8);
	
	//Create child processes that will do the updates
		if ((pid = fork()) == -1) 
	{
		//fork failed!
		perror("fork");
		exit(1);
	}
	
	if (pid == CHILD){
	//First Child Process. Dear old dad tries to do some updates.
		N=5;

		for(i=1;i<=N; i++)
		{

            //sets dad_ask to one because dad wants to update
            fp8 = fopen("dad_ask" , "r+");
		    fscanf(fp8, "%d", &dad_ask);
            fseek(fp8,0L, 0);
		    dad_ask += 1;
		    fprintf(fp8, "%d\n", dad_ask);
		    fclose(fp8);

            printf("Dear old dad is trying to do update.\n");

            //wait
            P(sem);

			fp1 = fopen("balance", "r+");
			fscanf(fp1, "%d", &bal2);
			printf("Dear old dad reads balance = %d \n", bal2);
			
			//Dad has to think (0-14 sec) if his son is really worth it
			sleep(rand()%15);
			fseek(fp1,0L,0);
			bal2 += 60;
			printf("Dear old dad writes new balance = %d \n", bal2);
			fprintf(fp1, "%d \n", bal2);
			fclose(fp1);

            //increases son 1's counter
            fp6 = fopen("son_1" , "r+");
	        fscanf(fp6, "%d", &son1_counter);
            fseek(fp6,0L, 0);
	        son1_counter += 1;
	        fprintf(fp6, "%d\n", son1_counter);
	        fclose(fp6);

            //increases son 2's counter
            fp7 = fopen("son_2" , "r+");
		    fscanf(fp7, "%d", &son2_counter);
            fseek(fp7,0L, 0);
		    son2_counter += 1;
		    fprintf(fp7, "%d\n", son2_counter);
		    fclose(fp7);            
    
            //sets dad_ask to 0 because dad finished update
            fp8 = fopen("dad_ask" , "r+");
		    fscanf(fp8, "%d", &dad_ask);
            fseek(fp8,0L, 0);
		    dad_ask -= 1;
		    fprintf(fp8, "%d\n", dad_ask);
		    fclose(fp8);

			printf("Dear old dad is done doing update.\n");
            
            //signal
            V(sem);

			sleep(rand()%5);	/* Go have coffee for 0-4 sec. */

		}
	}
	
	else
	{
		//Parent Process. Fork off another child process.
		if ((pid = fork()) == -1)
		{
			//Fork failed!
			perror("fork");
			exit(1);
		}
		if (pid == CHILD)
		{
			printf("First Son's Pid: %d\n",getpid());
			//Second child process. First poor son tries to do updates.
			flag = FALSE;
			while(flag == FALSE) 
			{

                //wait
                P(sem);

                fp3 = fopen("attempt" , "r+");
				fscanf(fp3, "%d", &N_Att);

				if(N_Att == 0)
				{
					fclose(fp3);
					flag = TRUE;
				}
				else
				{
                    //increases son 2 counter
                    fp7 = fopen("son_2" , "r+");
				    fscanf(fp7, "%d", &son2_counter);
                    fseek(fp7,0L, 0);
				    son2_counter += 1;
				    fprintf(fp7, "%d\n", son2_counter);
				    fclose(fp7);

					printf("Poor SON_1 wants to withdraw money.\n");
					fp2 = fopen("balance", "r+");
					fscanf(fp2,"%d", &bal2);
					printf("Poor SON_1 reads balance. Available Balance: %d \n", bal2);
					if (bal2 == 0)
					{
						fclose(fp2);
						fclose(fp3);
					}
					else
					{
						sleep(rand()%5);
						fseek(fp2,0L, 0);
						bal2 -=20;
						printf("Poor SON_1 write new balance: %d \n", bal2);
						fprintf(fp2,"%d\n", bal2);
						fclose(fp2);
						printf("poor SON_1 done doing update.\n");
						fseek(fp3,0L, 0);
						N_Att -=1;
						fprintf(fp3, "%d\n", N_Att);
						fclose(fp3);
					}
				}

                //increases dad counter if dad_ask = 1
                fp8 = fopen("dad_ask" , "r+");
				fscanf(fp8, "%d", &dad_ask);

                if(dad_ask != 0){
                    fp5 = fopen("dad" , "r+");
				    fscanf(fp5, "%d", &dad_counter);
                    fseek(fp5,0L, 0);
				    dad_counter += 1;
				    fprintf(fp5, "%d\n", dad_counter);
				    fclose(fp5);
                }

                fclose(fp8);

                //signal
                  V(sem);
			}
		}
		else
		{
		//Parent Process. Fork off one more child process.
			if ((pid = fork()) == -1) 
			{
				//fork failed!
				perror("fork");
				exit(1);
			}
			if (pid == CHILD)
			{
				printf("Second Son's Pid: %d\n",getpid());
				//Third child process. Second poor son tries to do updates.
				flag1 = FALSE;
				while(flag1 == FALSE) 
				{
                    
                    //wait
                    P(sem);

                    fp3 = fopen("attempt" , "r+");
					fscanf(fp3, "%d", &N_Att);

					if(N_Att == 0)
					{
						fclose(fp3);
						flag1 = TRUE;
					}
					else
					{   
                        //increases son 1 counter
                        fp6 = fopen("son_1" , "r+");
				        fscanf(fp6, "%d", &son1_counter);
                        fseek(fp6,0L, 0);
				        son1_counter += 1;
				        fprintf(fp6, "%d\n", son1_counter);
				        fclose(fp6);

						printf("Poor SON_2 wants to withdraw money.\n");
						fp2 = fopen("balance", "r+");
						fscanf(fp2,"%d", &bal2);
						printf("Poor SON_2 reads balance. Available Balance: %d \n", bal2);
						if (bal2 == 0)
						{
							fclose(fp2);
							fclose(fp3);
						}
						else
						{
							sleep(rand()%5);
							fseek(fp2,0L, 0);
							bal2 -=20;
							printf("Poor SON_2 write new balance: %d \n", bal2);
							fprintf(fp2,"%d\n", bal2);
							fclose(fp2);
							printf("poor SON_2 done doing update.\n");
							fseek(fp3,0L, 0);
							N_Att -=1;
							fprintf(fp3, "%d\n", N_Att);
							fclose(fp3);

						}

					}

                    //increases dad counter if dad_ask = 1
                    fp8 = fopen("dad_ask" , "r+");
					fscanf(fp8, "%d", &dad_ask);

                    if(dad_ask != 0){
                        fp5 = fopen("dad" , "r+");
				        fscanf(fp5, "%d", &dad_counter);
                        fseek(fp5,0L, 0);
				        dad_counter += 1;
				        fprintf(fp5, "%d\n", dad_counter);
				        fclose(fp5);
                    }
                    fclose(fp8);

                    //signal
                    V(sem);
				}
			}
			else
			{
				//Now parent process waits for the child processes to finish
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);

                semkill (sem);

                printf("N: %d \n", 5);

                //displays how many turns the dad had to wait before it was able to enter CS
                fp5 = fopen("dad" , "r+");
			    fscanf(fp5, "%d", &dad_counter);
                printf("Dad Total T(P): %d \n", dad_counter);
                fclose(fp5);

                //displays how many turns son 1 had to wait before it was able to enter CS
                fp6 = fopen("son_1" , "r+");
			    fscanf(fp6, "%d", &son1_counter);
                printf("Son 1 Total T(P): %d \n", son1_counter);
                fclose(fp6);

                //displays how many turns son 2 had to wait before it was able to enter CS
                fp7 = fopen("son_2" , "r+");
		        fscanf(fp7, "%d", &son2_counter);
                printf("Son 2 Total T(P): %d \n", son2_counter);
                fclose(fp7);
			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
}

