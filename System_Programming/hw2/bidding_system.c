#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_SET 3*7*11*13+11

int c[15] = {0}, set_num=1;
int set[MAX_SET][8];

void keep_set(){
    int i;
    for (i = 0; i < 8; i++){
        set[set_num][i] = c[i + 1];
    }
    set_num++;
}
void comp(int m, int player_num){
    if (m == 8 + 1){
        keep_set();
    }
    else{
        for (c[m] = c[m - 1] + 1; c[m] <= player_num - 8 + m; c[m]++){
            comp(m + 1, player_num);
        }
    }
}

int find_host(int k,int host_num, int key[]){
    for(int i = 1; i <= host_num; i++)
        if(k == key[i]) return i;
    return -1;
}

int main(int argc, char *argv[]){
	pid_t pid[15];
	int host_num, player_num,count=0,who;
	char read_buf[256],write_buf[256];
	int writefifo[15];
	int fd_read,i,j;
	FILE *fp_write[15],*fp_read;
	int rank[15],score[15],key[15];

	host_num = atoi(argv[1]), player_num= atoi(argv[2]);
	comp(1,player_num);
	for(i=set_num; i<set_num + 11; i++){
		for(j=0; j<8; j++){
			set[i][j] = -1;
		}
	}
	for(i=1; i<=player_num; i++){
		score[i]=0;
		rank[i]=1;
	}
	
	char *file_name[15];
	file_name[0]="./Host.FIFO";
	file_name[1]="./Host1.FIFO";
	file_name[2]="./Host2.FIFO";
	file_name[3]="./Host3.FIFO";
	file_name[4]="./Host4.FIFO";
	file_name[5]="./Host5.FIFO";
	file_name[6]="./Host6.FIFO";
	file_name[7]="./Host7.FIFO";
	file_name[8]="./Host8.FIFO";
	file_name[9]="./Host9.FIFO";
	file_name[10]="./Host10.FIFO";
	umask(0);
	for(i=0; i<=host_num; i++){
		unlink(file_name[i]);
		mkfifo(file_name[i], 0666);
		key[i]=i*19;
	}
	/*for(i=1;i<set_num+11;i++){
		fprintf(stderr, "%d %d %d %d %d %d %d %d\n",set[i][0],set[i][1],set[i][2],set[i][3],set[i][4],set[i][5],set[i][6],set[i][7]);
	}*/
	for(i=1; i<=host_num; i++){
		if((pid[i] = fork()) < 0){
			perror("fork\n");
		}
		else if( pid[i] == 0){  // for child
			char buf1[32], buf2[32];
            sprintf(buf1, "%d", i);
            sprintf(buf2, "%d", key[i]);
			execl("./host", "./host", buf1, buf2,"0", NULL);
		}
		else{ //parent give all host 1 comp
			writefifo[i] = open(file_name[i], O_WRONLY);
			if(writefifo[i] < 0) perror("writefifo_open failed");
			if((fp_write[i] = fdopen(writefifo[i], "w"))==NULL) perror("fp_write error");	
		}
	}

	for(i=1; i<=host_num; i++){
		if(fprintf(fp_write[i], "%d %d %d %d %d %d %d %d\n",set[i][0],set[i][1],set[i][2],set[i][3],set[i][4],set[i][5],set[i][6],set[i][7])<0){
			perror("give error");
		}
		fflush(fp_write[i]);
	}

	// for parent
	if( (fd_read = open(file_name[0], O_RDONLY)) < 0)
		perror("fd_read\n");
	if( (fp_read = fdopen(fd_read, "r")) == NULL)
		perror("fp_read\n");
	
	count=1;
	while(count < set_num){
		//fprintf(stderr, "count = %d, set_num = %d\n",count, set_num);
		int the_p,rank_p,i,who,key_now;
		fscanf(fp_read, "%d", &key_now);
		for(j=1; j<=8;j++){
			fscanf(fp_read, "%d%d", &the_p,&rank_p);
			score[the_p] += (8-rank_p);
		}
		//if(count  < set_num - host_num){
			who = find_host(key_now, host_num, key);
			if(who==-1) perror("find_host\n");
			//fprintf(stderr, "count + host_num = %d\n",count+host_num);
			sprintf(write_buf, "%d %d %d %d %d %d %d %d\n",set[count+host_num][0],set[count+host_num][1],set[count+host_num][2],set[count+host_num][3],set[count+host_num][4],set[count+host_num][5],set[count+host_num][6],set[count+host_num][7]);
			write(writefifo[who],write_buf,strlen(write_buf));
			fsync(writefifo[who]);
		count++;
	}
	//host end
	/*for(i=1;i<=host_num;i++){
		fprintf(fp_write[i],"-1 -1 -1 -1 -1 -1 -1 -1\n");
	}*/
	for(i=1; i<=player_num; i++){
		for(j=1; j<=player_num; j++){
			if(score[i]<score[j]){
				rank[i]++;
			}
		}
	}
	for(i=1; i<=player_num; i++){
		sprintf(write_buf, "%d %d\n", i, rank[i]);
		write(1, write_buf, strlen(write_buf));
	}
	for(i=0; i<=10; i++){
		unlink(file_name[i]);
	}
	/*for(i=0; i<=host_num; i++){
		fclose(fp_write[i]);
	}*/
	fclose(fp_read);
	/*for(i=1; i<=host_num; i++){
		wait(NULL);
	}*/
	return 0;
}