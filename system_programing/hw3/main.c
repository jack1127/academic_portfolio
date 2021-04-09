#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "scheduler.h"
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#define SIGUSR3 SIGWINCH

int main(){
	sigset_t set;
	int i,P,Q,R,fd[2];
	pid_t pid;
	char buffer[10],array[10000],p[3],q[3];
	// initialize
	scanf("%d%d", &P,&Q); //read the input
	scanf("%d", &R);
	sprintf(p,"%d",P);
	sprintf(q,"%d",Q);
	int signal[R];
	if(pipe(fd)<0){
		printf("create pipe error\n");
	}
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	sigaddset(&set, SIGUSR3);
	sigprocmask(SIG_BLOCK, &set, NULL);
	for(i=0; i<R; i++){
		scanf("%d", &signal[i]);
	}
	if((pid=fork()) == 0){
		close(fd[0]);
		dup2(fd[1], 1);
		if(execl("./hw3", "./hw3", p, q, "3", "0", NULL) < 0){
			printf("exec error\n");
		}
	}
	//close(fd[1]);
	else{
		for (i=0; i<R; i++){
			//printf("signal[%d]=%d\n",i,signal[i]);
			sleep(5);
			if(signal[i] == 1){
				kill(pid, SIGUSR1); // send signal
				read(fd[0], buffer, sizeof(buffer)); // read any message;
				//printf("sig1 : %s\n",buffer);
			}
			else if(signal[i] == 2){
				kill(pid, SIGUSR2);
				read(fd[0], buffer, sizeof(buffer)); // read any message;
				//printf("sig2 : %s\n",buffer);
			}
			else if(signal[i] == 3){
				kill(pid, SIGUSR3);
				fsync(fd[0]);
				strcpy(buffer, "");
				read(fd[0], buffer, sizeof(buffer)); // read ACK message;
				printf("%s\n", buffer);
			}
		}	
		read(fd[0], array, 10000); // read the content of arr
		printf("%s", array);
		waitpid(pid, NULL, WNOHANG); //
		close(fd[0]);
	}
	return 0;
}