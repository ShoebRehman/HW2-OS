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

int 		shmid; 	/*for external cleanup routine*/
int		matsize;
int main(argc, argv)
int	argc;
char	*argv[];
{
	int i, j, k, m, n, *pint, shmkey, offset;
	int status = 0;
	char *addr;
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
	/* now create walkers*/
	printf("Forking Children...\n");
	for (m = 0; m<matsize; m++){
		switch (walkpid = fork()) {
		case -1:  {
			perror("bad fork");
			exit(0);
		}
		case 0: {
			sprintf(thiswalkno, "%d", m);
			sprintf(thisstart, "%d", start);
			sprintf(thismatsize, "%d", matsize);
			sprintf(thisshmkey, "%d", shmkey);
			execlp
				(argv[3], "pidchild",
				thiswalkno,
				thisstart,
				thismatsize,
				thisshmkey,
				NULL);
		}
		default: {
			printf("Child %d sucessfully forked, waiting for start.\n", m + 1);
		}
		} /*end switch*/
	}/* end create walkers loop*/
	/* now signal to start*/

	pint = (int *)addr;
	*pint = atoi(argv[2]); /* restore true start*/
	/*wait for children to complete then terminate*/
	while ((wpid = wait(&status))>0);
	printf("All children released successfully.\n");
} /* end of main*/

