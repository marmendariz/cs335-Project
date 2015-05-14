#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int n1, n2, logfd, status;
char mystr[50];
pid_t cpid, ppid;

void SIGUSR1_handler (int sig) 
{
	if (sig = SIGUSR1) {
		write (logfd, mystr, strlen(mystr));
	}
}

int fib (int n)
{
	if (n < 2) return n;
	else return (fib(n - 1) + fib(n - 2));
}

int main (int argc, char *argv[]) 
{
	//int ret;
	sigset_t mask;
	sigfillset(&mask); // block all signals
	sigdelset(&mask, SIGINT); // unblock SIGINT 
	sigprocmask(SIG_SETMASK, &mask, NULL); // block signals until handler is set up 

	/*set up usr1 handler*/
	struct sigaction usr1;
	usr1.sa_handler = SIGUSR1_handler;
	usr1.sa_flags = SA_RESTART;
	sigfillset(&usr1.sa_mask);
	sigdelset(&usr1.sa_mask, SIGINT);
	//sigdelset(&usr1.sa_mask, SIGUSR1);

	if (sigaction(SIGUSR1, &usr1, NULL) == -1) { // register the mask to handler
		perror ("sigaction");
		exit (1);
	}

	if (argc < 4 || argc > 5) {
		printf ("Usage: ./70 <exitcode> <fibterm> <string>\n");
		exit(1);
	}

	n1 = atoi (argv[1]); // child's exit code
	n2 = atoi (argv[2]); // n to be passed to fib()

	memset (mystr, 0, 50);
	strcpy (mystr, argv[3]);
	char buf [50];
	memset (buf, 0, 50);	

	logfd = open("log", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (logfd < 0) {
		perror ("open logfd");
	}	

	printf("\n%s\n", "we made it to the fork");
	
	sigdelset(&mask, SIGUSR1);
	sigprocmask(SIG_SETMASK, &mask, NULL);
	
	cpid = fork();
	//ppid = getpid();

	if (cpid == 0) {

		ppid = getppid();

		int fibNum = fib (n2);
		kill(ppid, SIGUSR1);
		kill(ppid, SIGUSR2);
		exit (n1);
	}

	else {

		sigsuspend(&mask);
		//sigdelset(&mask, SIGUSR2);
		sigdelset(&mask, SIGCHLD);
		sigprocmask(SIG_SETMASK, &mask, NULL); // updating the mask
		wait(&status);
		if (WIFEXITED(status))
		{
			sprintf(buf, "WHAT THE HECK the exit status is %d\n", WEXITSTATUS(status));
			write(logfd, buf, strlen(buf));
		}
		//write(logfd, status, strlen(&status)); 
		close(logfd);
		exit(EXIT_SUCCESS); 
	}
	
	return 0;
}
