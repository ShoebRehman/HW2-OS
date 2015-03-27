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
#include	<pthread.h>
#define 	KFOUR 4096

struct releasepids{
	int* loc; 
	int* pidnum;
	char* addr;
	int* size;
};

int allocate_pid(char* addr, int size, int walkno, struct releasepids args);
void *release_pid(void* arguments);

int main(argc, argv)
	int	argc;
	char	*argv[];
	{
	int i,  *pint,  shmkey ;
	char *addr;
	
	int walkno, start,shmid,  matsize ;
	struct releasepids args;
	/* get parms*/
	walkno=atoi(argv[1]);
	start=atoi(argv[2]);
	matsize=atoi(argv[3]);
	shmkey=atoi(argv[4]);

	/*set up shared memory*/
	
/*wait for block on the first memory location to be cleared*/
	pint=(int *)addr;
        while(*pint > start)
		pint=(int *)addr;

	args.pidnum = walkno+1;
	args.addr = addr;
	*args.size = matsize;
	
	for(int j = 0; j < matsize; j++){
		pthread_create(NULL, NULL, thread, (void *) args);
	}
} /* end of main*/                  

void thread(void* arguments){
	struct releasepids *args = (struct releasepids *)arguments;
	allocate_pid(args->addr, args->size, args->pidnum);
	sleep(rand()%10);
	release_pid(
}

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
			return 0;
		}
	}
	return 1;
}

void *release_pid(char* addr, ){
	int *baseAdd;	
	baseAdd =(int *)addr;

	srand(time(NULL));
	sleep(rand() % 5);
	printf("Child %d has been released\n", *args->pidnum); 
	*addr = 0;
}

