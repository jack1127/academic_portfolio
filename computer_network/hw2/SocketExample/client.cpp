#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <iostream>
#include "opencv2/opencv.hpp"

#define BUFF_SIZE 1024

using namespace std;
using namespace cv;

int main(int argc , char *argv[])
{
    char client_buffer_const[BUFF_SIZE+17] = {};
    strcpy(client_buffer_const, "./client_buffer/");
    char client_buffer[BUFF_SIZE+17] = {};

    int localSocket, recved, sent, cmd;
    localSocket = socket(AF_INET , SOCK_STREAM , 0);
    
    char sendMessage[BUFF_SIZE] = {};
    char receiveMessage[BUFF_SIZE] = {};
    char stopMessage[] = {"stop"};
    char existMessage[] = {"exist"};
    char file_buffer[BUFF_SIZE] = {};
    char *filename;
    char *SM[2], *RM[2], *tmp;
    char *ptr, *ptr2;
    int idx = 0;
    int nCount;

    if (localSocket == -1){
        printf("Fail to create a socket.\n");
        return 0;
    }

    struct sockaddr_in info;
    bzero(&info, sizeof(info));

    info.sin_family = PF_INET;
    info.sin_addr.s_addr = inet_addr("127.0.0.1");
    info.sin_port = htons(4097);

    int err = connect(localSocket, (struct sockaddr *)&info, sizeof(info));
    if(err == -1){
        printf("Connection error\n");
        return 0;
    }

    while(1){
        cmd = 0;
        strcpy(client_buffer, client_buffer_const);
        cin.getline(sendMessage, BUFF_SIZE);
        if ((sent = send(localSocket, sendMessage, BUFF_SIZE,0)) < 0){
            cout << "sent failed, with sent bytes = " << sent << endl;
            break;
        }
        else if (sent == 0){  //end connection with server
            cout << "<end>\n";
            break;
        }
        
        idx = 0;
        ptr = sendMessage;
        tmp = NULL;

        while((tmp = strtok(ptr, " "))!=NULL){  
            SM[idx] = tmp;
            if (idx == 0){
                if (strcmp(SM[0], "ls") == 0){
                    cmd = 1;
                }
                else if (strcmp(SM[0], "put") == 0){  //upload file
                    cmd = 2;
                }
                else if (strcmp(SM[0], "get") == 0){  //download file
                    cmd = 3;
                }
                else if (strcmp(SM[0], "play") == 0){  //play video
                    cmd = 4;
                }
                else if (strcmp(SM[0], "exit") == 0){  //exit
                    cmd = 5;
                }
                else{
                    cmd = 0;
                    break;
                }   
            }
            else if (idx == 1){
                filename = strcat(client_buffer, SM[1]);
                strcpy(file_buffer, SM[1]);
            }
            idx++;
            ptr = NULL;
        }
        if(cmd == 0){
            cout << "Command not found.\n" ;
            continue;
        }
        
        else if (cmd == 5){  //exit
            printf("close Socket\n");
            close(localSocket);
            break;
        }

        else if(cmd == 1){  //ls
            bzero(receiveMessage, sizeof(char)*BUFF_SIZE);
            while( (recved = recv(localSocket, receiveMessage, BUFF_SIZE, 0)) > 0){
                if(strcmp(receiveMessage, "stop")==0){
                    break;
                }
                printf("%s\n", receiveMessage);
                bzero(receiveMessage, sizeof(char)*BUFF_SIZE);
            }
            printf("complete to ls!\n");
            continue;
        }

        else if(cmd == 2){  //put
            FILE *fp = fopen(filename, "rb");
            if(fp == NULL){
                printf("Cannot open file!\n");
                send(localSocket, stopMessage, BUFF_SIZE, 0);
                continue;
            }
            send(localSocket, existMessage, BUFF_SIZE, 0);
            while( (nCount = fread(sendMessage, 1, BUFF_SIZE, fp)) > 0 ){
                send(localSocket, sendMessage, nCount, 0);
            }
            
            printf("complete to put file!\n");
            fclose(fp);
            continue;
        }

        else if(cmd == 3){  //get           
            bzero(receiveMessage, sizeof(char)*BUFF_SIZE);
            recv(localSocket, receiveMessage, BUFF_SIZE, 0);		
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
            while( (recved = recv(localSocket, receiveMessage, sizeof(char)*BUFF_SIZE, 0)) > 0){
                //printf("receiveMessage : %s\n", receiveMessage);
                fwrite(receiveMessage, sizeof(char), recved, fp);
                if(recved < 1024){
                    //printf("stop\n");
                    break;
                }
                bzero(receiveMessage, sizeof(char)*BUFF_SIZE);
            }
            printf("complete to get file!\n");
            fclose(fp);
            continue;
        }

        else if(cmd == 4){  //play
            Mat imgClient;
            
            if( !(file_buffer[strlen(file_buffer)-3] == 'm') || !(file_buffer[strlen(file_buffer)-2] == 'p') || !(file_buffer[strlen(file_buffer)-1] == 'g')){
                printf("The %s is not a mpg file\n", file_buffer);
                send(localSocket, stopMessage, BUFF_SIZE, 0);
                continue;
            }
            send(localSocket, existMessage, BUFF_SIZE, 0);
                 
            bzero(receiveMessage, sizeof(char)*BUFF_SIZE);
            recv(localSocket, receiveMessage, BUFF_SIZE, 0);		
            if(strcmp(receiveMessage, "stop") == 0){
                printf("The file is not exist\n");
                continue;
            }
            
            // receive height, width from server
            bzero(receiveMessage, sizeof(char)*BUFF_SIZE);
            recv(localSocket, receiveMessage, BUFF_SIZE, 0);
            int width = atoi(receiveMessage);

            bzero(receiveMessage, sizeof(char)*BUFF_SIZE);
            recv(localSocket, receiveMessage, BUFF_SIZE, 0);
            int height = atoi(receiveMessage);
            bzero(receiveMessage, sizeof(char)*BUFF_SIZE);     
            recv(localSocket, receiveMessage, BUFF_SIZE, 0); 
            
            int imgSize = atoi(receiveMessage);
            
            imgClient = Mat::zeros(height, width, CV_8UC3);
            
            if(!imgClient.isContinuous()){
                imgClient = imgClient.clone();
            }
            
            uchar *iptr = imgClient.data;

            while(1){
                     
            	bzero(receiveMessage, sizeof(char)*BUFF_SIZE);
            	recv(localSocket, receiveMessage, BUFF_SIZE, 0);		
            	if(strcmp(receiveMessage, "stop") == 0){
                    printf("The end\n");
                    destroyAllWindows();
                    exit(0);
            	}

		//Press ESC on keyboard to exit
                // notice: this part is necessary due to openCV's design.
                // waitKey means a delay to get the next frame.
                char c = (char)waitKey(33.3333);
                if(c==27){
                    send(localSocket, stopMessage, BUFF_SIZE, 0); //interrupt server
                    printf("interrupt\n");
                    destroyAllWindows();
                    exit(0);
                }

                recv(localSocket, iptr, imgSize, MSG_WAITALL);
                imshow("Video", imgClient);  //client play

                
           }
           
           destroyAllWindows();   
        }
    }

    return 0;
}
