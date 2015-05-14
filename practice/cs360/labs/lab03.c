#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h> 

int main(void) {

	int status, logfd;
	pid_t cpid;	
	key_t ipckey;
	/*struct {
		long type;
		char text[100];
	}mymsg;*/	
	
	logfd = open("log", O_WRONLY|O_CREAT|O_TRUNC, 0644);
	if (logfd == -1) {
		perror("File open error: ");
	}

	char pathname[128];
	int shmid; // may or may not be the same after the fork
	int mqid;
	char buf[100];
	int received;

	//memset(buf,0,100);
	
	getcwd(pathname,128);
	strcat(pathname,"/foo");
	printf("%s \n", pathname);
	
	int myNum = 18;
	ipckey = ftok(pathname, myNum);
	
	if (ipckey == -1) {
			perror("ipckey error: ");
			exit(EXIT_FAILURE);
	}
	else {
		printf("My key is %d \n", ipckey); 
	}
	
	shmid = shmget(ipckey, sizeof(int), IPC_CREAT | 0666);
	int *shared = shmat(shmid, (void *) 0, 0);
	*shared = 0;

	mqid = msgget(ipckey, IPC_CREAT | 0666);
	if (mqid < 0) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	struct {
		long type;
		char text[100];
	}mymsg;	
	
	cpid = fork ();
	
	if (cpid == 0) {
		while (*shared == 0) {}

		/*Write the shared memory to the buffer, print the buffer to the log file*/
		sprintf(buf, "%d\n", *shared);
		write(logfd, buf, strlen(buf));	

		/* got the message */
		received = msgrcv(mqid, &mymsg, sizeof(mymsg), 0, 0);

		/*write the message to the log file */
		write(logfd, mymsg.text, strlen(mymsg.text));

		//shmdt(shared)
		close(logfd);
		shmdt(shared);
		exit(0);
	}

	else {
		memset(buf, 0, 100);
		read(0,buf,3);
		*shared = atoi(buf);

		memset(mymsg.text, 0, 100);
		memset(buf, 0, 100);

		read(0, buf, 100);

		strcpy(mymsg.text, buf);
		mymsg.type = 1;
		msgsnd(mqid, &mymsg, sizeof(mymsg), 0);
		wait(&status);
		shmdt(shared);

		msgctl(mqid, IPC_RMID, 0);
		shmctl(shmid, IPC_RMID, 0);
		exit(0);
	}
}	
