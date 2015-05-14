/*  dining.c
 *  a solution to the dining philosophers problem using mutexes 
 *
 *  code uses the C ternary conditional operator '?', which is
 *  a shorthand if-else statement: n = (flag==1) ? 10 : 20;  
 *  meaning: if flag is 1 then n is assigned value 10 otherwise value 20.
 *
 *   ./dining     // runs by quickly
 *   ./dining 40  // slows things down to view kernel thread scheduling
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LIMIT 5   /* this is the number of times each philosopher can eat */

// semaphore union
union {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *_buf;
} my_semun;

/* this structure will be passed to each thread function */ 
typedef struct philData {
  int philno;  /* philosopher number */
  pthread_mutex_t *forklft, *forkrgt;
  const char *name;
  pthread_t thread_id;
  int fail;
} Philosopher;

const char *nameList[] = {"Kant", "Hegel", "Plato", "Aristotle", "Decartes"};

/* count times each philosopher eats to verify no starvation */
int cnt[5];

int n = 1; /* pass this value to fib */

/* thread function */ 
void *philfunc(void *p) {
  int eat_cnt, failed, tries_left;
  int turn_cnt = LIMIT;  /* number of times each phil gets to eat */
  Philosopher *phil = p;
  pthread_mutex_t *forklft, *forkrgt, *forktmp;
 
  while (turn_cnt > 0) {
      fib(n);   /* make n large if you want to view kernel thread scheduling */
      forklft = phil->forklft;
      forkrgt = phil->forkrgt;
      tries_left = 2;   /* try twice before being forceful */

      do {
        	/* block on left fork grab */
        			
					//failed = pthread_mutex_lock(forklft);
		
			grab[forklft][0].sem_flg = 0;
			grab[forklft][1].sem_flg = 0;
			failed = semop(semid, grab[forklft], 2);
  
	   /* if you made it here you know you have the left fork so try right */

			if (tries_left > 0) {
				grab[forkrgt][0].sem_flg = IPC_NOWAIT;
				grab[forkrgt][1].sem_flg = IPC_NOWAIT;
				failed = semop (semid, grab[forkrgt], 2);
			}
			else {
				grab[forkrgt][0].sem_flg = 0;
				grab[forkrgt][1].sem_flg = 0;
				failed = semop (semid, grab[forkrgt], 2);
			}

			if (failed) {
				semop (semid, release[forklft], 1);
				forktmp = forklft;
				forklft = forkrgt;
				forkrgt = forktmp;
				tries_left -= 1;
			}
		} while((turn_cnt && failed < 0));

		if (!failed) {
			cnt[phil->philno]++;
			eat_cnt = cnt[phil->philno];
			printf("%9s %6hu taking turn %d\n", phil->name, phil->thread_id, eat_cnt);
			fflush(stdout);
	
			semop(semid, release[forklft], 1);
			semop(semid, release[forkrgt], 1);
			turn_cnt--;
		}
	}

	pthread_exit(0);
}

int main(int argc, char *argv[])  {
  int i,failed;
  if (argc == 2)
     n=atoi(argv[1]);

  /* initialize count array */
  for (i = 0; i<= 4; i++) 
     cnt[0] = 0;
  
  Philosopher philptrs[5];
  Philosopher *phil;
 
  semid = semget(IPC_PRIVATE, nsems, 0666 | IPC_CREAT);

  my_sem.val = 0;
  
  for (i = 0; i < 5; i++) {
  	semctl(semid, i, SETVAL, my_semun);

	grab[i][0].sem_num = i;
	grab[i][0].sem_flg = SEM_UNDO;
	grab[i][0].sem_op = 0;
  	
	grab[i][1].sem_num = i;
	grab[i][1].sem_flg = SEM_UNDO;
	grab[i][1].sem_op = +1;
  
	release[i][0].sem_num = i;
	release[i][0].sem_flg = SEM_UNDO;
	release[i][0].sem_op = -1;
  
	}
  	
  for (i = 0; i < 5; i++) {
      phil = &philptrs[i];
      phil->name = nameList[i];
      phil->philno = i;
      phil->forklft = &forks[i];
      phil->forkrgt = &forks[(i+1) % 5];
      phil->fail = pthread_create( &phil->thread_id, NULL, philfunc, phil);
  }
  /* join all the threads with main thread */ 
  for(i=0; i < 5; i++) {
     phil = &philptrs[i];
     if (pthread_join(phil->thread_id, NULL)<0) {
        printf("error joining thread for %s", phil->name);
      }
      else printf("joined %s who ate %d times.\n",phil->name,cnt[phil->philno]);
  }

  semctl (semid, 0, IPC_RMID);
  exit(0);
}

int fib( int n) {
  if (n==0) return 0;
  if (n==1) return 1;
  return fib(n-1) + fib(n-2);
}

