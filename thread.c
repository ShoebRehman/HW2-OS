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

char *addr;
int *pint;

int 		shmid; 	/*for external cleanup routine*/
int		matsize;
int main(argc, argv)
int	argc;
char	*argv[];
{
	int i, j, k, m, n, shmkey, offset;
	int status = 0;
	int  walkers, walkpid, next, start;
	char thisstart[10];
	char thiswalkno[10];
	char thismatsize[10];
	char thisshmkey[10];
	pid_t wpid;

	printf("Shoeb Rehman - 23018971\n\n");
	/* start processing with test of args*/
	if (argc<4)
	{
		perror("Not enough parameters:  basename, matsize, pidchild location, start");
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
	
	for(long int j = 0; j < matsize; j++){
		pthread_t thread;
		pthread_create(&thread, NULL, thread, (void *) j);
	}
	
	pint = (int *)addr;
	*pint = atoi(argv[2]); /* restore true start*/
	/*wait for children to complete then terminate*/
	while ((wpid = wait(&status))>0);
	printf("All children released successfully.\n");
} /* end of main*/

void thread(int number){
	pint=(int *)addr;
    while(*pint > start)
		pint=(int *)addr;
	
	int pid = allocate_pid(number);
	sleep(rand()%10);
	release_pid(pid);
}

int allocate_pid(int number){
	int *baseAdd;	
	baseAdd =(int *)addr;
	
	printf("Child %d is waiting for a PID...\n\t", number+1);	
	for(int i = 1; i < size+1; i++){
		if(*baseAdd != 0){
			baseAdd++;
		}		

		if(*baseAdd == 0){
			printf("Child %d took PID %d\n", number+1 , i); 
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
	*addr = 0;
}
