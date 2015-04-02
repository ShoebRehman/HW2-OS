/* mod 5 8 99 for shuffled start compile with -lm flag */
#define _XOPEN_SOURCE 700
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include	<stdio.h>
#include	<unistd.h>
#include 	<stdlib.h>
#include 	<pthread.h>
#define	KEY 75
#define KFOUR  4096

int allocate_pid(int number);
char* memInit(int shmkey);
void release_pid(int pidnum);
void *threadCreate(int pid);
void print();
void threadExec();

char *addr;
int *pint;
int start;

int 		shmid; 	/*for external cleanup routine*/
int		matsize;
pthread_mutex_t mut;

int main(argc, argv)
int	argc;
char	*argv[];
{	
	/* start processing with test of args*/
	if (argc<2)
	{
		perror("Not enough parameters: matsize, start");
		exit(0);
	}
	/* get parms*/
	matsize = atoi(argv[1]);
	start = atoi(argv[2]);
	

	addr = memInit(KEY); //initializes shared memory segment and returns base address
	threadExec(); //creation of threads, must wait for start

	printf("\n\tStarting Threads\n\n");	

	*pint = atoi(argv[2]); /*Starts threads*/
	
	

	pthread_exit(NULL);//Waits for threads to finish
	printf("All threads executed successfully.\n");
	return 0;
} /* end of main*/

void *threadCreate(int childNum){
	int pid;
	
	while(*pint != start){
	}
	
	/*Acquire Lock*/
	pthread_mutex_lock(&mut);
	
	pid = allocate_pid(childNum);
	
	/*Release Lock*/
	pthread_mutex_unlock(&mut);
	
	sleep(rand() % 5);
	
	
	/*Acquire Lock*/
	pthread_mutex_lock(&mut);

	release_pid(pid);
	
	/*Release Lock*/
	pthread_mutex_unlock(&mut);	
}

int allocate_pid(int number){
	int* baseAdd;
	baseAdd = (int *)addr;
	printf("Thread %d is waiting for a PID...\n\t", number+1);	
	for(int i = 0; i < matsize+1; i++){
		if(*baseAdd == 0){
			printf("Thread %d took PID %d\n", number+1 , i+1); 
			*baseAdd = number+1;
			return i+1;
		}
		baseAdd++;
	}
}

void release_pid(int pidnum){
	int childNum;
	int* baseAddr;
	baseAddr =(int *)addr;
	baseAddr+=pidnum;
	baseAddr--;
	childNum = *baseAddr;
	printf("Thread %d has released PID %d successfully\n", childNum, pidnum); 
	*baseAddr = 0;
}

void print(){
	pint=(int *)addr;
	int i;
	for (i=0; i<matsize; i++){
		pint++;
		printf("%d\t", *pint);
		}
	printf("\n");
}

char* memInit(int key){
	printf("Shoeb Rehman - 23018971\n\n");
	shmid = shmget(key, KFOUR, 0777 | IPC_CREAT);
	addr = shmat(shmid, 0, 0);
	

	printf("Starting at Memory Location: 0x%x\n", addr);
	pint = (int *)addr;

	printf("Initializing Shared Memory...\n");
	for (int i = 0; i<matsize; i++)  {
		pint++;
		*pint = 0;
	}
	printf("Done.\n");
	
	return addr;
}

void threadExec(){
	pthread_t thread[matsize];
	for(long int j = 0; j < matsize; j++){
		pthread_create(&thread[j], NULL, threadCreate, (void *)j);
		printf("Thread %d created, waiting for start\n", j+1);	
	}
}
