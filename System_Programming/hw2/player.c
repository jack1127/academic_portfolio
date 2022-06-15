#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int player_num,i;
	char read_buf[256],write_buf[256];
	player_num = atoi(argv[1]);
	sprintf(write_buf, "%d %d\n", player_num, player_num*100);
	write(1, write_buf, strlen(write_buf));
	for(i=0; i<9; i++){
		read(0, read_buf, sizeof(read_buf));		
		write(1, write_buf, strlen(write_buf));
	}
}