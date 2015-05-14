	#include <unistd.h>
  #include <stdio.h>
   #include <stdlib.h>
   #include <errno.h>
   #include <sys/types.h>
   #include <sys/stat.h>
  #include <sys/wait.h>
  #include <fcntl.h>
  
  // global variables
  int status, logfd;
  pid_t cpid, parent;
  
  void myhandler (int);
  
  int main () {
  
      struct sigaction sa; // sets up the structure that has sa_mask, sa_handler and sa_handler
  
      sigset_t mask;
      sigfillset(&mask); // doesn't do anything, but under the covers sets a variable
      sigprocmask(SIG_BLOCK, &mask, NULL); // SIG_BLOCK means block all of the signals in &mask (add to them to the existing mask)  
  
      sa.sa_handler = myhandler; // set the sa_handler to the signal handler specifically made 
      sa.sa_flags = 0; // allows for the reset
      sigfillset(&sa.sa_mask); // setting the mask to be all ones
  
      if(sigaction(SIGUSR1, &sa, NULL) == -1) { // link the handler to the signal
          perror("sigaction");
          exit(0);
      }
  
      int cpid = fork(); // clone a new process
  
      if(cpid == 0) { // CHILD    
          logfd = open("log", O_CREAT | O_WRONLY | O_TRUNC, 0644);
          write(logfd,"hello ",6);
          sigdelset(&mask, SIGUSR1); // The mask was full (blocking everything),then unblock SIGUSR1 
          sigsuspend(&mask); // the only signal that will wake up child is SIGUSR1; this is a process mask sent via sigsuspend so won't deadlock
          write(logfd," world\n",7);
          close(logfd);
          exit(0);
      }
  
      else { // PARENT
          kill(cpid, SIGTERM); // parent sending signals to child
          kill(cpid, SIGUSR1);
          wait(&status); // wait to harvest the child's exit status, but if child isn't killed by signal, then the procmask was passed and the parent can be terminated by sigusr1
  
          if (WIFSIGNALED(status))
              printf("Child terminated by signal %d\n", WTERMSIG(status));
          else
              printf("Child exited normally with exit code %d\n", WEXITSTATUS(status));
          exit(EXIT_SUCCESS);
      }
  }
    
   void myhandler (int sig) {
          sigfillset(&sa.sa_mask); 
			if (cpid == 0) {
          write(logfd, "Got Signal", 10);
      }
  }
                                                                                                                          
