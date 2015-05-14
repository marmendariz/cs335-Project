/* Lab 01
 *
 *Basis derived from:
 * fork4.c     

  demonstrate how a child becomes an orphan; when the parent dies before 
  harvesting the child's exit code the child becomes an orphan and is 
  inherited by init; init harvests the code (no zombie)

  Compile, execute and view log file:

     $ make  
        OR 
     $ gcc -o fork4 fork4.c
     $ ./fork4
     $ cat log
 */

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int n;

int fib(int n) {
  if (n<2) return n;
  else return fib(n-1) + fib(n-2);
}

int main(int argc, char *argv[]) {

	
	int status, ret;
	pid_t cpid,parent;
	parent = getppid();

	if (argc == 2) {
		n = atoi(argv[1]);
	}

	else { 
		printf("Usage: ./lab01 n\n");
		exit(1);
	}
	 
	cpid = fork();  /* fork returns pid of child to the parent, 0 to child */

   /* CHILD */
   if (cpid == 0) {  /* 0 means you are the child */

      char buf[100];
	  int logfd, fibNum;   
      pid_t me;
      me = getpid();
      parent = getppid();

      logfd = open("log", O_WRONLY|O_CREAT|O_TRUNC, 0644);  /* open log */

      fibNum = fib(n);
 
	time_t timer; 
	struct tm *tm_info;
      time(&timer);
      tm_info = localtime(&timer);
      strftime(buf, 26, "%Y:%m:%d %H:%M:%S\n", tm_info);
      write(logfd,buf,strlen(buf));

      /* grab pids after wait - parent should be init */
      sprintf(buf,"fib: %d is: %d\n",n,fibNum); 
      write(logfd,buf,strlen(buf));

      /* sync the file or strange things happen when the parent dies */
      fsync(logfd);
      close(logfd);

      exit(n); 
   }

   /* PARENT */
   else {

      /* parent does some busy work before dying - long enough for the child to
         write its first entry to log */
      
	char buf[100];
	wait(&status); // wait for the child to die
	
	sprintf(buf,"my child exited with code: %d\n ", WEXITSTATUS(status));
	write(1, buf, strlen(buf));  // (output to screen, address of string ,number of characters 

      exit(0);
   }
}
