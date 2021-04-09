#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "scheduler.h"
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#define SIGUSR3 SIGWINCH

char arr[10000];
int idx, P, Q, task, loop_num;
int loop1,loop2,loop3,loop4;
jmp_buf back, SCHEDULER, HANDLER, HANDLER3;
int mutex;
sigset_t set;
int queue[5];

FCB fnc[6];
FCB_ptr Current, Head;

void handler(int signo){
	int i; 
	if(signo==SIGUSR1 || signo==SIGUSR2){
		char buffer12[10];
		strcpy(buffer12, "");
		sigprocmask(SIG_BLOCK, &set, NULL);
		sprintf(buffer12, "fu");
		write(1, buffer12, sizeof(buffer12));
		fflush(stdout);
		longjmp(SCHEDULER, 6);
	}
	else if(signo == SIGUSR3){
		char buffer3[10];
		strcpy(buffer3, "");
		int qidx=0, inqueue[5];
		for(i=1; i<=4; i++){
			if(queue[i]==1){
				inqueue[qidx]=i;
				qidx++;
			}
		}
		for(i=0; i<qidx; i++){
			if(i==qidx-1){
				sprintf(buffer3+strlen(buffer3), "%d", inqueue[i]);
			}
			else{
				sprintf(buffer3+strlen(buffer3), "%d ", inqueue[i]);
			}
		}
		write(1, buffer3, sizeof(buffer3));

		Current=Current->Previous;
		sigprocmask(SIG_BLOCK, &set, NULL);

		longjmp(SCHEDULER, 6);
	}
}

void funct_1(int name){
	int i, j;
	// do the longjmp or setjmp
	if(setjmp(fnc[1].Environment)==0){
		funct_5(2);
	}
	if(task == 3){
		if(mutex == 0 || mutex == 1){
			for(j = loop1; j <= P; j++) // We call this for loop as “Big loop” in the following description
			{	
				for(i = 1; i <= Q; i++) // We call this for loop as “Small loop” in the following description
				{
					mutex=1;
					sleep(1); // You should sleep for a second before append to arr
					arr[idx++] = '1';
				}
				loop1++;
				queue[1] = 0;
				sigset_t pend;
				// use sigpending and sigismember to check which signal is pending
				sigpending(&pend);
				// if SIGUSR2 is pending, release the lock
				if(sigismember(&pend, SIGUSR1)){
					//perror("sig 1 in 1");
					sigprocmask(SIG_UNBLOCK, &set, NULL);
				}
				else if(sigismember(&pend, SIGUSR2)){
					mutex = 0;
					//perror("sig 2 in 1");
					sigprocmask(SIG_UNBLOCK, &set, NULL);
				}
				else if(sigismember(&pend, SIGUSR3)){
					//perror("sig 3 in 1");
					sigprocmask(SIG_UNBLOCK, &set, NULL);
				}		
			}
		}
		else{
			//put funct_1 into the queue
			queue[1]= 1;
			longjmp(SCHEDULER, 6);
		}
	}
	else{
		if(mutex == 0 || mutex == 1){
			for(j = loop1; j <= P; j++) // We call this for loop as “Big loop” in the following description
			{	
				for(i = 1; i <= Q; i++) // We call this for loop as “Small loop” in the following description
				{
					if(task==2) mutex=1;
					sleep(1); // You should sleep for a second before append to arr
					arr[idx++] = '1';
				}
				//something else	
				if(task==2 && loop1 % loop_num == 0){
					mutex = 0;
					loop1++;
					longjmp(SCHEDULER, 1);
				}
				loop1++;
			}
		}
	}
	mutex=0;
	longjmp(SCHEDULER, -2);
}

void funct_2(int name){
	int i, j;
	// do the longjmp or setjmp
	if(setjmp(fnc[2].Environment)==0){
		funct_5(3);
	}
	if(task == 3){
		if(mutex == 0 || mutex == 2){
			for(j = loop2; j <= P; j++) // We call this for loop as “Big loop” in the following description
			{	
				for(i = 1; i <= Q; i++) // We call this for loop as “Small loop” in the following description
				{
					mutex=2;
					sleep(1); // You should sleep for a second before append to arr
					arr[idx++] = '2';
				}
				loop2++;
				queue[2]=0;
				sigset_t pend;
				// use sigpending and sigismember to check which signal is pending
				sigpending(&pend);
				// if SIGUSR2 is pending, release the lock
				if(sigismember(&pend, SIGUSR1)){
					//perror("sig 1 in 2");
					sigprocmask(SIG_UNBLOCK, &set, NULL);
				}
				else if(sigismember(&pend, SIGUSR2)){
					mutex = 0;
					//perror("sig 2 in 2");
					sigprocmask(SIG_UNBLOCK, &set, NULL);
				}
				else if(sigismember(&pend, SIGUSR3)){
					//perror("sig 3 in 2");
					sigprocmask(SIG_UNBLOCK, &set, NULL);
				}
			}
		}
		else{
			//put funct_1 into the queue
			queue[2]= 1;
			longjmp(SCHEDULER, 6);
		}
	}
	else{
		if(mutex == 0 || mutex == 2){
			for(j = loop2; j <= P; j++) // We call this for loop as “Big loop” in the following description
			{	
				for(i = 1; i <= Q; i++) // We call this for loop as “Small loop” in the following description
				{
					if(task==2) mutex=1;
					sleep(1); // You should sleep for a second before append to arr
					arr[idx++] = '2';
				}
				//something else	
				if(task==2 && loop2 % loop_num == 0){
					mutex = 0;
					loop2++;
					longjmp(SCHEDULER, 2);
				}
				loop2++;
			}
		}
	}
	mutex=0;
	longjmp(SCHEDULER, -2);
}

void funct_3(int name){
	int i, j;
	// do the longjmp or setjmp
	if(setjmp(fnc[3].Environment)==0){
		funct_5(4);
	}
	if(task == 3){
		if(mutex == 0 || mutex == 3){
			for(j = loop3; j <= P; j++) // We call this for loop as “Big loop” in the following description
			{	
				for(i = 1; i <= Q; i++) // We call this for loop as “Small loop” in the following description
				{
					mutex=3;
					sleep(1); // You should sleep for a second before append to arr
					arr[idx++] = '3';
				}
				loop3++;
				queue[3]=0;
				sigset_t pend;
				// use sigpending and sigismember to check which signal is pending
				sigpending(&pend);
				// if SIGUSR2 is pending, release the lock
				if(sigismember(&pend, SIGUSR1)){
					//perror("sig 1 in 3");
					sigprocmask(SIG_UNBLOCK, &set, NULL);
				}
				else if(sigismember(&pend, SIGUSR2)){
					//perror("sig 2 in 3");
					mutex = 0;
					sigprocmask(SIG_UNBLOCK, &set, NULL);
				}
				else if(sigismember(&pend, SIGUSR3)){
					//perror("sig 3 in 3");
					sigprocmask(SIG_UNBLOCK, &set, NULL);
				}
			}
		}
		else{
			//put funct_1 into the queue
			queue[3]= 1;
			longjmp(SCHEDULER, 6);
		}
	}
	else{
		if(mutex == 0 || mutex ==3){
			for(j = loop3; j <= P; j++) // We call this for loop as “Big loop” in the following description
			{	
				for(i = 1; i <= Q; i++) // We call this for loop as “Small loop” in the following description
				{
					if(task==2) mutex=3;
					sleep(1); // You should sleep for a second before append to arr
					arr[idx++] = '3';
				}
				//something else	
				if(task==2 && loop3 % loop_num == 0){
					mutex = 0;
					loop3++;
					longjmp(SCHEDULER, 3);
				}
				loop3++;
			}
		}
	}
	mutex=0;
	longjmp(SCHEDULER, -2);
}

void funct_4(int name){
	int i, j;
	// do the longjmp or setjmp
	if(setjmp(fnc[4].Environment)==0){
		longjmp(back, 4);
	}
	if(task == 3){
		if(mutex == 0|| mutex == 4){
			for(j = loop4; j <= P; j++) // We call this for loop as “Big loop” in the following description
			{	
				for(i = 1; i <= Q; i++) // We call this for loop as “Small loop” in the following description
				{
					mutex=4;
					sleep(1); // You should sleep for a second before append to arr
					arr[idx++] = '4';
				}
				loop4++;
				queue[4]=0;
				sigset_t pend;
				// use sigpending and sigismember to check which signal is pending
				sigpending(&pend);
				// if SIGUSR2 is pending, release the lock
				if(sigismember(&pend, SIGUSR1)){
					//perror("sig 1 in 4");
					sigprocmask(SIG_UNBLOCK, &set, NULL);
				}
				else if(sigismember(&pend, SIGUSR2)){
					mutex = 0;
					//perror("sig 2 in 4");
					sigprocmask(SIG_UNBLOCK, &set, NULL);
				}
				else if(sigismember(&pend, SIGUSR3)){
					//perror("sig 3 in 4");
					sigprocmask(SIG_UNBLOCK, &set, NULL);
				}
			}
		}
		else{
			//put funct_1 into the queue
			queue[4]= 1;
			longjmp(SCHEDULER, 6);
		}
	}
	else{
		if(mutex == 0 || mutex ==4){
			for(j = loop4; j <= P; j++) // We call this for loop as “Big loop” in the following description
			{	
				for(i = 1; i <= Q; i++) // We call this for loop as “Small loop” in the following description
				{
					if(task==2) mutex=4;
					sleep(1); // You should sleep for a second before append to arr
					arr[idx++] = '4';
				}
				//something else	
				if(task==2 && loop4 % loop_num == 0){
					mutex = 0;
					loop4++;
					longjmp(SCHEDULER, 4);
				}
				loop4++;
			}
		}
	}
	mutex=0;
	longjmp(SCHEDULER, -2);
}

void funct_5(int name){
	int a[10000]; // This line must not be changed.
	// call other functions
	switch (name){
		case 1 :
			funct_1(1);
			break;
		case 2 :
			funct_2(2);
			break;
		case 3 :
			funct_3(3);
			break;
		case 4 :
			funct_4(4);
			break;
	}
	return;
}

int main(int argc, char *argv[]){
	int k;
	P = atoi(argv[1]), Q = atoi(argv[2]), task = atoi(argv[3]), loop_num = atoi(argv[4]);
	fnc[1].Name = 1;
	fnc[1].Next = &fnc[2];
	fnc[1].Previous = &fnc[4];
	fnc[2].Name = 2;
	fnc[2].Next = &fnc[3];
	fnc[2].Previous = &fnc[1];
	fnc[3].Name = 3;
	fnc[3].Next = &fnc[4];
	fnc[3].Previous = &fnc[2];
	fnc[4].Name = 4;
	fnc[4].Next = &fnc[1];
	fnc[4].Previous = &fnc[3];
	Head = &fnc[1];
	Current = &fnc[4];
	loop1 = loop2 = loop3 = loop4 = 1;
	if(setjmp(back)==0){  //fnc4 longjmp back to here
		funct_5(1);
	}
	if(task == 3){
		sigemptyset(&set);
		signal(SIGUSR1, handler);
		signal(SIGUSR2, handler);
		signal(SIGUSR3, handler);
		sigaddset(&set, SIGUSR1);
		sigaddset(&set, SIGUSR2);
		sigaddset(&set, SIGUSR3);
		sigprocmask(SIG_BLOCK, &set, NULL);
	}
	Scheduler();
	return 0;
}