/* compile with -lm for rand*/
#define _XOPEN_SOURCE 700
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include	<sys/signal.h> 
#include 	<unistd.h>       
#include	<stdio.h>
#include	<math.h>
#define 	KFOUR 4096


int allocate_pid(char* addr, int size, int walkno);
void release_pid(int* loc, int pidnum);

int main(argc, argv)
	int	argc;
	char	*argv[];
	{
	int i,  *pint,  shmkey ;
	char *addr;
	int walkno, start,shmid,  matsize ;
	/* get parms*/
	walkno=atoi(argv[1]);
	start=atoi(argv[2]);
	matsize=atoi(argv[3]);
	shmkey=atoi(argv[4]);

	/*set up shared memory*/
	shmid=shmget(shmkey, KFOUR, 0777);
	addr=shmat(shmid,0,0);

/*wait for block on the first memory location to be cleared*/
	pint=(int *)addr;
        while(*pint > start)
		pint=(int *)addr;

	allocate_pid(addr, matsize, walkno);

/*print out state of array*/
	/*pint=(int *)addr;
	printf( "child %d pint %d *pint %d\n",walkno, pint, *pint);
	for(i=0; i<matsize; i++){
		pint++;
		printf("%d\t", *pint);
		}
		printf("\n");
	printf ("child %d exiting \n", walkno+1);	
	return(0);
	*/
} /* end of main*/                  

int allocate_pid(char* addr, int size, int walkno){
	int *baseAdd;	
	baseAdd =(int *)addr;
	printf("Child %d is waiting for a PID...\n\t", walkno+1);	
	for(int i = 1; i < size+1; i++){
		if(*baseAdd != 0){
			baseAdd++;
		}		

		if(*baseAdd == 0){
			printf("Child %d took PID %d\n", walkno+1, i); 
			*baseAdd = walkno+1;
			release_pid(baseAdd, walkno+1);
			return 0;
		}
	}
	return 1;
}

void release_pid(int* loc, int pidnum){
	int* addr = loc;
	srand(time(NULL));
	sleep(rand() % 5);
	printf("Child %d has been released\n", pidnum); 
	*addr = 0;
}

