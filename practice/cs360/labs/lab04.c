/* lab04.c
 * 
 * This file uses shared memory. If you do not understand how shared memory 
 * works, review week 03 examples. Your job in lab 04 is to add a semaphore 
 * to control order. You know things have gone awry when you run the code:
 *  
 *    $ make lab04
 *    $ ./lab04
 *    child reads: 0
 *
 * The desired result is for the parent to compute fib(n), write the result to
 * shared memory then the child reads the result and displays it to the screen.
 * The problem is that things are out of order - by the time the parent computes
 * fib(10) the child has already read memory; i.e., the parent modifies the 
 * segment too late.
 * 
 * This scenario is a race condition. For example, if you pass a small enough 
 * number to fib, the child generally grabs the value OK:
 *
 *    $ ./lab04 10
 *    child reads: 55 
 *
 * To fix this problem you need to add a semaphore to control order. You want 
 * the parent to grab the semaphore first. Meanwhile the child is blocked on 
 * the semaphore. After the parent writes fib(n) to memory the parent releases 
 * the semaphore and the child can then grab it.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/sem.h>

int status;
#define BUFSIZE 256

int fib(int);

int main(int argc, char **argv) {

    int n;
    char buf[BUFSIZE];
    pid_t cpid;
    int shmid; 
    int *shared;
	int sem_value;
	int logfd;

	/* define increment semaphore*/
	struct sembuf inc[1];
	inc[0].sem_num = 0;
	inc[0].sem_flg = SEM_UNDO;
	inc[0].sem_op = +1;
	 
	/*define decrement semaphore*/
	struct sembuf dec[1];
	dec[0].sem_num = 0;
	dec[0].sem_flg = SEM_UNDO;
	dec[0].sem_op = -1;

    /* check if n was passed */
    if (argc == 2)
      n = atoi(argv[1]);
    else 
      n = 20;

	/*open log file*/
	logfd = open("log", O_WRONLY | O_CREAT | O_TRUNC, 0644);	

	/* IPC_PRIVATE will provide a unique key without using an ipckey 
     * it works with related processes but not unrelated ones - it is
     * a safe way to get a ipckey to use in a fork */
    shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);

	int nsems = 1;
	int semid = semget (shmid, nsems, 0666 | IPC_CREAT);

	if (semid < 0) {
		printf ("Error - %s\n", strerror(errno));
		_exit(1);
	}

    /* attach and initialize memory segment */
    shared = shmat(shmid, (void *) 0, 0);
    *shared = 0;

    cpid = fork();

    /* CHILD */
    if (cpid == 0) { 

       /* Attach to shared memory - both child and parent must do this but
        * the parent can do it before the fork */
       shared = shmat(shmid, (void *) 0, 0);
        
       /* child reads and displays shared memory */

	   //block the child by decrementing
		semop(semid, dec, 1);

       int val = *shared;
       sprintf(buf,"child reads: %d\n",val);
       write(logfd, buf, strlen(buf));

        /* detach from segment */
        shmdt(shared); 

		/*safe to close log file here*/
		close (logfd);
        
		exit(0);
    }
    else { /* PARENT */

        /* parent computes fib(n) and writes it to shared memory */
        *shared=fib(n);
        semop (semid, inc, 1);
		wait(&status); 

        /* detach from segment */
        shmdt(shared);

        /* remove shared segment */
        shmctl(shmid, IPC_RMID, 0);
	  	
		semctl(semid, 0, IPC_RMID);	
   }
}

/* some busy work for the parent */
int fib(int n) {
 if (n < 2) return n;
 return fib(n-1) + fib(n-2);
}
