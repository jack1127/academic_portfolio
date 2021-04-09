#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>

unsigned char x[60000][784], xT[784][60000], xtest[10000][784], y[60000][10];
double w[784][10], y_hat[60000][10], wgrad[784][10], ans[10000][10];
double maxinrow[60000], lr=0.00005;

void init(){
	for(int i=0; i<60000; i++){
		for(int j=0; j<10; j++){
			//printf("y_hat = %lf  ", y_hat[i][j]);
			y_hat[i][j]=0;
		}
	}
	for(int i=0; i<784; i++){
		for(int j=0; j<10; j++){
			//printf("wgrad = %f  ", wgrad[i][j]);
			wgrad[i][j]=0;
		}
	}
}

void op2(int arr[]){  //softmax
	//x*w
	for (int i = arr[0]; i < arr[1]; i++){
        for (int k = 0; k < 784; k++) {
            double r = x[i][k];
            for (int j = 0; j < 10; j++){
                y_hat[i][j] += r * w[k][j];        
            }
        }
    }
}
void op1(){
    //find max in row
    for(int i=0; i<60000; i++){
    	for(int j=0; j<10; j++){
    		if(j==0) maxinrow[i] = y_hat[i][0];
    		else if(y_hat[i][j] > maxinrow[i]) maxinrow[i] = y_hat[i][j];
    	}
    }
    
    //exp
	for(int i=0; i < 60000; i++){
		double tempsum = 0;
		for(int j=0; j<10; j++){	
			y_hat[i][j] = exp(y_hat[i][j]-maxinrow[i]);
			tempsum += y_hat[i][j];
		}
		for(int j=0; j<10; j++){
			y_hat[i][j]/=tempsum;
		}
	}
	return;
}

void op3(){  // update w,   lr can set in 10^-4 ~ 10^-6
	for(int i = 0; i<784; i++){
		for(int j=0; j<10; j++){
			w[i][j]= w[i][j] - lr * wgrad[i][j];
		}
	}
	return;
}

void op4(){  // Wgrad
	for (int i = 0; i < 784; i++){
        for (int k = 0; k < 60000; k++) {
            double r = xT[i][k];
            for (int j = 0; j < 10; j++){
                wgrad[i][j] += r * (y_hat[k][j]-y[k][j]);
            }
        }
    }
    return;
}

void* thr_func(void *vptr){
	int *arr = (int *)vptr;
	op2(arr);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	int i,j,k, fd, test;
	unsigned char label;
	void *tret;
	pthread_t tid[100];
	int thnum = atoi(argv[4]);
	int range[thnum][2], size=60000/thnum;
	for(i=0; i < thnum; i++){
		if(i==0){
			range[i][0] = 0;
			range[i][1] = size;
		}
		else{
			range[i][0] = range[i-1][1]+1;
			range[i][1] = range[i-1][1]+size;
		}
	}
	fd = open(argv[1], O_RDONLY);
 	for (int i = 0; i < 60000; i++)
		read(fd, &x[i], sizeof(x[i]));
	close(fd);
	fd = open(argv[2], O_RDONLY);
	for (int i = 0; i < 60000; i++) {
		read(fd, &label, sizeof(label));
		y[i][label] = 1;
	}
	close(fd);
	//read test data
	fd = open(argv[3], O_RDONLY);
	for(i=0; i<10000; i++){
		read(fd, &xtest[i], sizeof(xtest[i]));
	}
	close(fd);
	//transpose x
	for(i=0;i<60000; i++){
		for(j=0; j<784; j++){
			xT[j][i] = x[i][j];
		}
	}
 	
	//train w
	for(i=0; i<30; i++){
		for(j=0; j<thnum; j++){
			pthread_create(&tid[j], NULL, thr_func, &range[j]);
		}
		for(j=0; j<thnum; j++){
			pthread_join(tid[j], &tret);
		}
		op1();
		op4();
		op3();
		lr*=0.95;
		init();
	}

	// test
	init();
	for (i = 0; i < 10000; i++){
        for (k = 0; k < 784; k++) {
            double r = xtest[i][k];
            for (j = 0; j < 10; j++){
                ans[i][j] += r * w[k][j];   
            }
        }
    }
    freopen("result.csv", "w", stdout);
    //check test result
    printf("id,label\n");
    for(i=0; i<10000; i++){
    	double tempmax;
    	int tempidx;
    	for(j=0; j<10; j++){
    		if(j==0){
    			tempidx = 0;
    			tempmax = ans[i][0];
    		} 
    		else if(ans[i][j] > tempmax){
    			tempidx = j;
    			tempmax = ans[i][j];
    		}
    		//printf("ans = %f\n", ans[i][j]);
    	}
    	printf("%d,%d\n",i,tempidx); 
    }

	return 0;
}