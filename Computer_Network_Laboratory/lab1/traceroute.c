#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/time.h>

void dtostrf(double val, int len, int prec, char* buf, int buf_len) {
    int int_count = 0;
    int index = 0;
    //判斷正負
    if (val < 0) {
        buf[index] = '-';
        index ++;
        val = -val;
    }
    //先數一下整數部分長度
    while ((int) val > 0) {
        val /= 10.0;
        int_count++;
    }
    while (index < len && index < buf_len) {
        if (int_count == 0) {
            //如果一上來就是0
            if (index == 0) {
                buf[index] = '0';
                index++;
            }
            else {
                buf[index] = '.';
                index++;
                int_count--;
            }
        }
        //int_count此時已經是小數長度的相反數
        else if ( int_count >= -prec) {
            val *= 10.0;
            buf[index] = (int)val + '0';
            val -= (double)((int)val);
            index++;
            int_count--;
        }
        else break;
    }
    //強制加'\0'
    if (index == buf_len)  buf[index - 1] = '\0';
    else buf[index] = '\0';
}

char *DNSLookup(char *hostname) {
    struct hostent *host; 
    char *ip=(char*)malloc(NI_MAXHOST*sizeof(char)); 
    if ((host = gethostbyname(hostname)) == NULL) { 
        return NULL; 
    }   
    strcpy(ip, inet_ntoa(*(struct in_addr *)host->h_addr)); 
    return ip;
}

unsigned short checksum(unsigned short *input, int len){
    
    unsigned int sum = 0;
    unsigned short result = 0;

    while(len > 1){
        sum += *input;
        input++;
        len -= 2;
    }

    if(len == 1)
        sum += *(unsigned char*)input;

    sum = (sum >> 16) + (sum & 65535);
    result = sum + (sum >> 16);
    result = ~result;
    return result;
}

int main(int argc, char *argv[]) {
    char *dest = argv[1];
    char *ip = DNSLookup(dest);
    
    if (ip == NULL) {
        printf("traceroute: unknown host %s\n", dest);
        exit(1);
    }
    
    int icmpfd;
    if ((icmpfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        printf("Can not open socket with error number %d\n", errno);
        exit(1);
    }
    
    struct sockaddr_in sendAddr;
    sendAddr.sin_port = htons (7);
    sendAddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &(sendAddr.sin_addr));
    
    // Set timeout
    // TODO
    struct timeval timeout;      
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (setsockopt(icmpfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
    {
        printf("Setting socket receive timeout failed!\n");
        exit(1);
    }


    int finish = 0; // if the packet reaches the destination
    int maxHop = 64; // maximum hops
    struct icmp sendICMP; 
    struct timeval begin, end; // used to record RTT
    int seq = 0; // increasing sequence number for icmp packet
    int count = 3; // sending count for each ttl
    char RTT[3][8];
    printf("traceroute to %s (%s), %d hops max\n", dest, ip, maxHop);
    for(int h = 1; h < maxHop; h++){
        // Set TTL
        // TODO
        int ttl = h;
        if (setsockopt(icmpfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
        {
            printf("Setting TTL failed!\n");
            exit(1);
        } 
        char hostname[4][128];
        char srcIP[4][32];

        for(int c = 0; c < count; c++){
            // Set ICMP Header
            // TODO
            sendICMP.icmp_type = ICMP_ECHO;
            sendICMP.icmp_code = 0;
            sendICMP.icmp_id = getpid();
            sendICMP.icmp_seq = seq++;
            sendICMP.icmp_cksum = 0;

            // Checksum
            // TODO
            sendICMP.icmp_cksum = checksum((unsigned short*)&sendICMP, sizeof(sendICMP));
            
            // Send the icmp packet to destination
            // TODO
            int status;
            if((status = sendto(icmpfd, &sendICMP, sizeof(sendICMP), 0, (struct sockaddr *)&sendAddr, sizeof(sendAddr))) < 0)
            {
                printf("Can not send packet with error number %d\n", errno);
                exit(1);
            }
            gettimeofday(&begin, NULL);
        
            // Recive ICMP reply, need to check the identifier and sequence number
            struct ip *recvIP;
            struct icmp *recvICMP;
            struct sockaddr_in recvAddr;
            u_int8_t icmpType;
            unsigned int recvLength = sizeof(recvAddr);
            char recvBuf[1500];
            float interval[4] = {};
            memset(&recvAddr, 0, sizeof(struct sockaddr_in));
            
            int recv = recvfrom(icmpfd, recvBuf, sizeof(recvBuf), 0, (struct sockaddr *)&recvAddr, &recvLength);
            gettimeofday(&end, NULL);
            float diff;
            diff = (end.tv_sec - begin.tv_sec)*1000.0 + (end.tv_usec - begin.tv_usec)/1000.0;
            if(diff > 1000){
                strcpy(RTT[c], "*");
            }
            else{
                dtostrf(diff, 8, 3, RTT[c], 8);
            }
            
            // Get source hostname and ip address 
            recvIP = (struct ip *)recvBuf;
            getnameinfo((struct sockaddr *)&recvAddr, sizeof(recvAddr), hostname[c], sizeof(hostname[c]), NULL, 0, 0); 
            strcpy(srcIP[c], inet_ntoa(recvIP->ip_src));
            
            if(strcmp(srcIP[c], ip) == 0){
                finish = 1;
            }
        }    

        // Print the result
        // TODO
        int ip_cnt = 0;
        if(strcmp(hostname[0], hostname[1]) == 0 && strcmp(hostname[1], hostname[2]) == 0 && strcmp(hostname[0], "") != 0)
            ip_cnt ++;

        if(strcmp(hostname[0], hostname[1]) != 0)
            ip_cnt ++;
        
        if(strcmp(hostname[0], hostname[2]) != 0 && strcmp(hostname[1], hostname[2]) != 0 )
            ip_cnt ++;
            
        switch(ip_cnt){
        case 0:
            printf("%2d  %s %s %s\n", h, RTT[0], RTT[1], RTT[2]);
            break;
        case 1 :
            printf("%2d  %s (%s) %s ms %s ms %s ms\n", h, hostname[0], srcIP[0], RTT[0], RTT[1], RTT[2]);
            break;
        case 2 :
            if(strcmp(hostname[0], hostname[1]) == 0){
                printf("%2d  %s (%s) %s ms %s ms\n", h, hostname[0], srcIP[0], RTT[0], RTT[1]);
                printf("    %s (%s) %s ms\n", hostname[2], srcIP[2], RTT[2]);
           }
            else if(strcmp(hostname[1], hostname[2]) == 0){
                printf("%2d  %s (%s) %s ms\n", h, hostname[0], srcIP[0], RTT[0]);
                printf("    %s (%s) %s ms %s ms\n", hostname[1], srcIP[1], RTT[1], RTT[2]);
            }
            else if(strcmp(hostname[0], hostname[2]) == 0){
                printf("%2d  %s (%s) %s ms %s ms\n", h, hostname[0], srcIP[0], RTT[0], RTT[2]);
                printf("    %s (%s) %s ms\n", hostname[1], srcIP[1], RTT[1]);
            }
            break;
        case 3 :
            printf("%2d  %s (%s) %s ms\n", h, hostname[0], srcIP[0], RTT[0]);
            printf("    %s (%s) %s ms\n", hostname[1], srcIP[1], RTT[1]);
            printf("    %s (%s) %s ms\n", hostname[2], srcIP[2], RTT[2]);
            break;
        }
        for (int i = 0; i < 3; i++) {
            strcpy(hostname[i], "");
            strcpy(srcIP[i], "");
        }
        if(finish){
            break;
        }
    }
    close(icmpfd);
    return 0;
}