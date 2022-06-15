#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

int pipe_left1[2],pipe_left2[2],pipe_right1[2],pipe_right2[2];  //1= parent to child , 2=child to parent
FILE *fp, *fp1, *fp2, *fp3, *fp4, *fp_write;
void mkpipe(){
	pipe(pipe_left1);
	pipe(pipe_left2);
	pipe(pipe_right1);
	pipe(pipe_right2);
}
void close_pipe() {
	close(pipe_left1[0]);
	close(pipe_left1[1]);
	close(pipe_left2[0]);
	close(pipe_left2[1]);
	close(pipe_right1[0]);
	close(pipe_right1[1]);
	close(pipe_right2[0]);
	close(pipe_right2[1]);
}
void open_fp(){
	fp1 = fdopen(pipe_left2[0], "r");
	fp2 = fdopen(pipe_right2[0], "r");
	fp3 = fdopen(pipe_left1[1], "w");
	fp4 = fdopen(pipe_right1[1], "w");	
}

int main(int argc, char *argv[]) {
	char *path_name[16];
	pid_t pid;
	path_name[1]  = "./Host1.FIFO";
	path_name[2]  = "./Host2.FIFO";
	path_name[3]  = "./Host3.FIFO";
	path_name[4]  = "./Host4.FIFO";
	path_name[5]  = "./Host5.FIFO";
	path_name[6]  = "./Host6.FIFO";
	path_name[7]  = "./Host7.FIFO";
	path_name[8]  = "./Host8.FIFO";
	path_name[9]  = "./Host9.FIFO";
	path_name[10] = "./Host10.FIFO";

	int host_id, random_key, depth;
	host_id = atoi(argv[1]);
	random_key = atoi(argv[2]);
	depth = atoi(argv[3]);

	if (depth == 0) {	
		fp = fopen(path_name[host_id], "r+");
		fp_write = fopen("./Host.FIFO", "w+");

		mkpipe();

		if ((pid = fork())< 0) {
			perror("fork error");
			exit(0);
		}
		else if (pid == 0) {
			dup2(pipe_left1[0], 0);
			dup2(pipe_left2[1], 1);
			close_pipe();
			execl("./host", "./host", argv[1], argv[2], "1", NULL);
		}
	
		if ((pid = fork()) < 0) {
			perror("fork error");
			exit(0);
		}
		else if (pid == 0) {
			dup2(pipe_right1[0], 0);
			dup2(pipe_right2[1], 1);
			close_pipe();
			execl("./host", "./host", argv[1], argv[2], "1", NULL);
		}

		open_fp();  
		int p[9], score[16];
		do{
			char buf1[16], buf2[16];
			for (int i = 1; i <= 14; i++)
				score[i] = 0;
			for (int i = 1; i <= 8; i++) {
				fflush(fp);
				fscanf(fp, "%d", &p[i]);
			}
			sprintf(buf1, "%d %d %d %d\n", p[1], p[2], p[3], p[4]);
			sprintf(buf2, "%d %d %d %d\n", p[5], p[6], p[7], p[8]);
			fprintf(fp3, "%s", buf1);
			fflush(fp3);
			fprintf(fp4, "%s", buf2);	
			fflush(fp4);
			
			if (p[1] == -1){
				break;
			}

			for (int i = 0; i < 10; i++) {
				int winner, player1, player2, money1, money2;
				fflush(fp1);
				fscanf(fp1, "%d%d", &player1, &money1);
				fflush(fp2);
				fscanf(fp2, "%d%d", &player2, &money2);
				if (money1 > money2)
					winner = player1;
				else
					winner = player2;
				score[winner]++;
				if (i == 9)
					break;
				fprintf(fp3, "%d\n", winner);
				fflush(fp3);
				fprintf(fp4, "%d\n", winner);	
				fflush(fp4);			
			}

			int rank[16];
			char buf[128];
			for (int i = 1; i <= 14; i++)
				rank[i] = 1;
			sprintf(buf, "%d\n",random_key);
			for (int i = 1; i <= 8; i++) {
				for (int j = 1; j <= 8; j++) {
					if (score[p[i]] < score[p[j]])
						rank[p[i]]++;
				}
				sprintf(buf + strlen(buf), "%d %d\n", p[i], rank[p[i]]);
			}
			fprintf(fp_write, "%s", buf);
			fflush(fp_write);
		}while(p[1] != -1);
		wait(NULL);
		wait(NULL);
		fclose(fp);
		fclose(fp_write);
		close_pipe();
		exit(0);
	}
	else if (depth == 1) {
		mkpipe();

		if ((pid = fork())< 0) {
			perror("fork error");
			exit(0);
		}
		else if (pid == 0) {
			dup2(pipe_left1[0], 0);
			dup2(pipe_left2[1], 1);
			close_pipe();
			execl("./host", "./host", argv[1], argv[2], "2", NULL);
		}
	
		if ((pid = fork()) < 0) {
			perror("fork error");
			exit(0);
		}
		else if (pid == 0) {
			dup2(pipe_right1[0], 0);
			dup2(pipe_right2[1], 1);
			close_pipe();
			execl("./host", "./host", argv[1], argv[2], "2", NULL);
		}

		open_fp();
		int p[9];
		do{
			char buf1[16], buf2[16];	
			for (int i = 1; i <= 4; i++) {
				fflush(stdin);
				fscanf(stdin, "%d", &p[i]);
			}	
			sprintf(buf1, "%d %d\n", p[1], p[2]);
			sprintf(buf2, "%d %d\n", p[3], p[4]);
			fprintf(fp3, "%s", buf1);
			fflush(fp3);
			fprintf(fp4, "%s", buf2);
			fflush(fp4);

			if (p[1] == -1)
				break;

			for (int i = 0; i < 10; i++) {
				int winner, player1, player2, money1, money2;
				fflush(fp1);
				fscanf(fp1, "%d%d", &player1, &money1);
				fflush(fp2);
				fscanf(fp2, "%d%d", &player2, &money2);
				if (money1 > money2) {
					fprintf(stdout, "%d %d\n", player1, money1);
					fflush(stdout);
				}
				else {
					fprintf(stdout, "%d %d\n", player2, money2);
					fflush(stdout);
				}
				if (i != 9){
					fflush(stdin);
					fscanf(stdin, "%d", &winner);
					fprintf(fp3, "%d\n", winner);
					fflush(fp3);
					fprintf(fp4, "%d\n", winner);	
					fflush(fp4);
				}
			}		
		}while(p[1] != -1);
		wait(NULL);
		wait(NULL);
		close_pipe();
		exit(0);	
	}
	else {
		int p[9];
		do {
			char buf1[16], buf2[16];
			for (int i = 1; i <= 2; i++) {
				fflush(stdin);
				fscanf(stdin, "%d", &p[i]);
			}

			if (p[1] == -1)
				break;

			mkpipe();

			if ((pid = fork()) < 0) {
				perror("fork error");
				exit(0);
			}
			else if (pid == 0) {
				char id[4];
				strcpy(id, "");
				dup2(pipe_left1[0], 0);
				dup2(pipe_left2[1], 1);
				close_pipe();
				fflush(stdin);
				fscanf(stdin, "%s", id);
				execl("./player", "./player", id, NULL);
			}
		
			if ((pid = fork()) < 0) {
				perror("fork error");
				exit(0);
			}
			else if (pid == 0) {
				char id[4];
				strcpy(id, "");
				dup2(pipe_right1[0], 0);
				dup2(pipe_right2[1], 1);
				close_pipe();
				fflush(stdin);
				fscanf(stdin, "%s", id);
				execl("./player", "./player", id, NULL);
			}

			open_fp();

			sprintf(buf1, "%d\n", p[1]);
			sprintf(buf2, "%d\n", p[2]);
			fprintf(fp3, "%s", buf1);
			fflush(fp3);
			fprintf(fp4, "%s", buf2);
			fflush(fp4);
			for (int i = 0; i < 10; i++) {
				int winner, player1, player2, money1, money2;
				fflush(fp1);
				fscanf(fp1, "%d%d", &player1, &money1);
				fflush(fp2);
				fscanf(fp2, "%d%d", &player2, &money2);
				if (money1 > money2) {
					fprintf(stdout, "%d %d\n", player1, money1);
					fflush(stdout);
				}
				else {
					fprintf(stdout, "%d %d\n", player2, money2);
					fflush(stdout);
				}
				if (i != 9){
					fflush(stdin);
					fscanf(stdin, "%d", &winner);
					fprintf(fp3, "%d\n", winner);
					fflush(fp3);
					fprintf(fp4, "%d\n", winner);	
					fflush(fp4);
				}
			}
			wait(NULL);
			wait(NULL);
			close_pipe();
		}while(p[1] != -1);
		exit(0);
	}
}