/* mod 5 8 99 for shuffled start compile with -lm flag */
#define _XOPEN_SOURCE 700
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include	<stdio.h>
#include	<unistd.h>
#include 	<stdlib.h>
#define	KEY 75
#define KFOUR  4096

int allocate_pid(int nunber);
void release_pid(int pidnum);
void *threadCreate(int number);

char *addr;
int* baseAdd;
int *pint;
int start;

int 		shmid; 	/*for external cleanup routine*/
int		matsize;

int main(argc, argv)
int	argc;
char	*argv[];
{
	int i, shmkey;
	int status = 0;
	pid_t wpid;

	printf("Shoeb Rehman - 23018971\n\n");
	/* start processing with test of args*/
	if (argc<2)
	{
		perror("Not enough parameters: matsize, start");
		exit(0);
	}
	/* get parms*/
	matsize = atoi(argv[1]);
	start = atoi(argv[2]);
	shmkey = KEY;
	/*set up shared memory*/
	shmid = shmget(shmkey, KFOUR, 0777 | IPC_CREAT);
	addr = shmat(shmid, 0, 0);
	printf("Starting at Memory Location: 0x%x\n", addr);
	pint = (int *)addr;

	printf("Initializing Shared Memory...\n");
	for (i = 0; i<matsize; i++)  {
		pint++;
		*pint = 0;
	}
	printf("Done.\n");
	
	pthread_t thread;
	
	for(long int j = 0; j < matsize; j++){
		pthread_create(&thread, NULL, threadCreate, (void *) j);
	}
	
	pint = (int *)addr;
	*pint = atoi(argv[2]); /* restore true start*/
	
	/*wait for children to complete then terminate*/
	while ((wpid = wait(&status))>0);
	printf("All children released successfully.\n");
	return 0;
} /* end of main*/

void *threadCreate(int number){
	int pid = 0;
	
	pint=(int *)addr;
    while(*pint > start){
		pint=(int *)addr;
	}
	pid = allocate_pid(number);
	
	//sleep(rand() % 5);
	
	//release_pid(pid);
	
}

int allocate_pid(int number){
	int *baseAdd;	
	baseAdd = (int *)addr;
	
	printf("Child %d is waiting for a PID...\n\t", number+1);	
	for(int i = 0; i < matsize; i++){
		if(*baseAdd != 0){
			baseAdd++;
		}		

		if(*baseAdd == 0){
			printf("Child %d took PID %d\n", number+1 , i+1); 
			*baseAdd = number+1;
			return i;
		}
	}
}

void release_pid(int pidnum){
	int childNum;
	int *baseAdd;	
	baseAdd =(int *)addr;
	baseAdd+=pidnum;
	childNum = *baseAdd;
	printf("Child %d has been released\n", childNum+1); 
	*baseAdd = 0;
}
