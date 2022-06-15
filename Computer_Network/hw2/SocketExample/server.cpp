#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"

#define BUFF_SIZE 1024  //revise to avoid file transmission error problem

using namespace std;
using namespace cv;

void *doInChildThread(void *ptr);

int main(int argc, char** argv){
    pthread_t pid[10];

    int localSocket, remoteSocket, port = 4097;                               

    struct  sockaddr_in localAddr,remoteAddr;
	          
    int addrLen = sizeof(struct sockaddr_in);  

    localSocket = socket(AF_INET , SOCK_STREAM , 0);

     if (localSocket == -1){
	printf("socket() call failed!!\n");
	return 0;
     }

    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons(port);

    char Message[BUFF_SIZE] = {};

	if( bind(localSocket,(struct sockaddr *)&localAddr , sizeof(localAddr)) < 0) {
		printf("Can't bind() socket\n");
		return 0;
	}
		        
	listen(localSocket , 3);
	
	for(int i = 0 ; i < 10; i++){
		int* input = new int(i);
		
		std::cout <<  "Waiting for connections...\n"
                <<  "Server Port:" << port << std::endl;
        
        remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);  
        
        if (remoteSocket < 0) {
            printf("accept failed!\n");
            return 0;
        }
                
        std::cout << "Connection accepted" << std::endl;

        // create a new thread.
		// pthread_create (ptr to a pthread_id, NULL, function to run, pointer to  the parameter)
		pthread_create(&pid[i], NULL, doInChildThread, &remoteSocket);
		// should be pthread_create(&pid[i], NULL, doInChildThread, &remoteSocket);
	}	

	
	for(int i = 0 ; i < 10; i++){
		// wait a thread to be finished
		// pthread_join(ptr to a pthread_id, NULL)
		pthread_join(pid[i],NULL);
	}
	return 0;
}

void *doInChildThread(void *arg){  //server 重複執行accept之後部分 丟進這裡
	int remoteSocket =  *(int*)arg;
	printf("Thread %d: print %d\n",remoteSocket,remoteSocket+1);
    
    int recved, sent, cmd, nCount;
    char sendMessage[BUFF_SIZE] = {};
	char receiveMessage[BUFF_SIZE] = {};
	char stopMessage[BUFF_SIZE] = {};
    strcpy(stopMessage, "stop");
    char existMessage[] = {"exist"};
    char server_buffer_ls[BUFF_SIZE+17] = {};
    char server_buffer[BUFF_SIZE+17] = {};
    strcpy(server_buffer_ls, "./server_buffer/");
    strcpy(server_buffer, "./server_buffer/");

    while(1){
        char *RM[2], *tmp;
        strcpy(server_buffer, server_buffer_ls);
        bzero(receiveMessage, sizeof(char)*BUFF_SIZE);
        if ((recved = recv(remoteSocket, receiveMessage, BUFF_SIZE,0)) < 0){
            cout << "recv failed, with received bytes = " << recved << endl;
            break;
        }
        else if (recved == 0){ //end connection with server
            //cout << "<end>\n";
            break;
        }

        printf("function : %s\n", receiveMessage);
        int idx = 0;
        char *ptr = receiveMessage;
        while((tmp = strtok(ptr, " "))!=NULL){
            RM[idx] = tmp;
            idx++;
            ptr=NULL;
        }

        //file transmission
        if (strcmp(RM[0], "exit") == 0){
        	printf("close thread %d: print %d\n", remoteSocket, remoteSocket+1);
            close(remoteSocket);
        	break;
        }
        if (strcmp(RM[0], "ls") == 0){  //print out files in server
            DIR* p_dir;
            struct dirent* p_next;

            if((p_dir = opendir(server_buffer_ls)) == NULL){
                perror("opendir error");
                exit(1);
            }

            while((p_next = readdir(p_dir)) != NULL){
                if(strcmp(p_next->d_name, ".") == 0 || strcmp(p_next->d_name, "..") == 0 ){
                    continue;
                }
                strcpy(sendMessage, p_next->d_name);
                //printf("sendMessage = %s.\n", sendMessage);
                send(remoteSocket, sendMessage, BUFF_SIZE, 0);
            }

            send(remoteSocket, stopMessage, BUFF_SIZE, 0);
            printf("done ls\n");
        }
        else if (strcmp(RM[0], "put") == 0){  //put
            char *filename = strcat(server_buffer, RM[1]);
           
            bzero(receiveMessage, sizeof(char)*BUFF_SIZE);
            recv(remoteSocket, receiveMessage, BUFF_SIZE, 0);
            if(strcmp(receiveMessage, "stop") == 0){
                printf("The file is not exist\n");
                continue;
            }

            FILE *fp = fopen(filename, "wb");
            if(fp == NULL){
                printf("Cannot open file!\n");
                fclose(fp);
                continue;
            }

            bzero(receiveMessage, sizeof(char)*BUFF_SIZE);
            while( (recved = recv(remoteSocket, receiveMessage, BUFF_SIZE, 0)) > 0){
                fwrite(receiveMessage, sizeof(char), recved, fp);
                if(recved < BUFF_SIZE)
                    break;
                bzero(receiveMessage, sizeof(char)*BUFF_SIZE);
            }
            printf("done put\n");
            fclose(fp);
        }

        else if (strcmp(RM[0], "get") == 0){  //get
            char *filename = strcat(server_buffer, RM[1]);
            FILE *fp = fopen(filename, "rb");
            if(fp == NULL){
                printf("Cannot open file!\n");
                send(remoteSocket, stopMessage, BUFF_SIZE, 0);
                continue;
            }
            send(remoteSocket, existMessage, BUFF_SIZE, 0);

            while( (nCount = fread(sendMessage, 1, BUFF_SIZE, fp)) > 0 ){
                send(remoteSocket, sendMessage, nCount, 0);
            }
 	
            /*if( (sent = send(remoteSocket, stopMessage, strlen(stopMessage), 0)) > 0){
                printf("stopMessage : %s. with %d\n", stopMessage, sent);
            }*/
            
            printf("done get\n");
            fclose(fp);
        }
	
        
        else if (strcmp(RM[0], "play") == 0){  //play video	
            char *filename = strcat(server_buffer, RM[1]);
            char interrupt[BUFF_SIZE] = {};
            
            bzero(receiveMessage, sizeof(char)*BUFF_SIZE);
            recv(remoteSocket, receiveMessage, sizeof(char)*BUFF_SIZE, 0);
            if(strcmp(receiveMessage, "stop") == 0){
                printf("The file is not a mpg file\n");
                continue;
            }

            // server

            Mat imgServer;
            VideoCapture cap(server_buffer);


            // get the resolution of the video
            int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
            int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
     
            //allocate container to load frames

            imgServer = Mat::zeros(height, width, CV_8UC3);
            
            if( imgServer.empty() ){
                printf("The video is not found\n");
                send(remoteSocket, stopMessage, BUFF_SIZE, 0);
                continue;
            }
            send(remoteSocket, existMessage, BUFF_SIZE, 0);
            
            bzero(sendMessage, sizeof(char)*BUFF_SIZE);
            sprintf(sendMessage, "%d", width);
            send(remoteSocket, sendMessage, BUFF_SIZE, 0);
 
            bzero(sendMessage, sizeof(char)*BUFF_SIZE);
            sprintf(sendMessage, "%d", height);
            send(remoteSocket, sendMessage, BUFF_SIZE, 0); //height width message to client

            if(!imgServer.isContinuous()){
                imgServer = imgServer.clone();
            }

            // get the size of a frame in bytes 
            int imgSize = imgServer.total() * imgServer.elemSize();
                
            bzero(sendMessage, sizeof(char)*BUFF_SIZE);
            sprintf(sendMessage, "%d", imgSize);
            send(remoteSocket, sendMessage, BUFF_SIZE, 0);

            while(1){

                //get a frame from the video to the container on server.
                cap >> imgServer;
                if( imgServer.empty() ){
                    printf("The end\n");
                    send(remoteSocket, stopMessage, BUFF_SIZE, 0);
                    break;
                }
                send(remoteSocket, existMessage, BUFF_SIZE, 0);
                send(remoteSocket, imgServer.data, imgSize, 0); //send from server
                
                bzero(interrupt, sizeof(char)*BUFF_SIZE);
                if ((recved = recv(remoteSocket, interrupt, BUFF_SIZE, MSG_DONTWAIT)) > 0 && (strcmp(interrupt, "stop")) == 0){ //interrupt from client
                    printf("interrupt\n");
                    break;
                }
            }

            printf("done play\n");
            cap.release();
        }

        else{
            cout << "Command not found.\n" ;
            continue;
        }

    }
    return 0;
}
