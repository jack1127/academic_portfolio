#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define ERR_EXIT(a) { perror(a); exit(1); }

typedef struct {
    char hostname[512];  // server's hostname
    unsigned short port;  // port to listen
    int listen_fd;  // fd to wait for a new connection
} server;

typedef struct {
    char host[512];  // client's host
    int conn_fd;  // fd to talk with client
    char buf[512];  // data sent by/to client
    size_t buf_len;  // bytes used by buf
    // you don't need to change this.
    int item;
    int wait_for_write;  // used by handle_read to know if the header is read or not.
} request;

typedef struct {
    int id;
    int balance;
} Account;

int to_lock_req(int fd, int cmd, int type, off_t offset, int whence, off_t len){
    struct flock lock;
    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;
    return(fcntl(fd, cmd, &lock));
}

int read_lock(int fd, off_t offset,int whence,off_t len){
    return to_lock_req(fd, F_SETLK, F_RDLCK, offset, whence, len);
}
int write_lock(int fd, off_t offset,int whence,off_t len){
    return to_lock_req(fd, F_SETLK, F_WRLCK, offset, whence, len);
}
int unlock(int fd, off_t offset, int whence, off_t len){
    return to_lock_req(fd, F_SETLK, F_UNLCK, offset, whence, len);
}

server svr;  // server
request* requestP = NULL;  // point to a list of requests
int maxfd;  // size of open file descriptor table, size of request list
int lock_status[20] = {0};

const char* accept_read_header = "ACCEPT_FROM_READ";
const char* accept_write_header = "ACCEPT_FROM_WRITE";

// Forwards

static void init_server(unsigned short port);
// initailize a server, exit for error

static void init_request(request* reqP);
// initailize a request instance

static void free_request(request* reqP);
// free resources used by a request instance

static int handle_read(request* reqP);
// return 0: socket ended, request done.
// return 1: success, message (without header) got this time is in reqP->buf with reqP->buf_len bytes. read more until got <= 0.
// It's guaranteed that the header would be correctly set after the first read.
// error code:
// -1: client connection error


int main(int argc, char** argv) {
    int i, j, ret;

    struct sockaddr_in cliaddr;  // used by accept()
    int clilen;

    int conn_fd;  // fd for a new connection with client
    int file_fd;  // fd for file that we open for reading
    char buf[512];
    int buf_len;

    // Parse args.
    if (argc != 2) {
        fprintf(stderr, "usage: %s [port]\n", argv[0]);
        exit(1);
    }

    // Initialize server
    init_server((unsigned short) atoi(argv[1]));

    // Get file descripter table size and initize request table
    maxfd = getdtablesize();
    requestP = (request*) malloc(sizeof(request) * maxfd);
    if (requestP == NULL) {
        ERR_EXIT("out of memory allocating all requests");
    }
    for (i = 0; i < maxfd; i++) {
        init_request(&requestP[i]);
    }
    requestP[svr.listen_fd].conn_fd = svr.listen_fd;
    strcpy(requestP[svr.listen_fd].host, svr.hostname);

    // Loop for handling connections
    fprintf(stderr, "\nstarting on %.80s, port %d, fd %d, maxconn %d...\n", svr.hostname, svr.port, svr.listen_fd, maxfd);

    file_fd=open("account_list",O_RDWR);
    fd_set readset,tmpset;
    FD_ZERO(&readset);
    FD_ZERO(&tmpset);
    FD_SET(svr.listen_fd, &readset);

    unlock(file_fd, 0, SEEK_SET, 20*sizeof(Account));

    while (1) {
        // TODO: Add IO multiplexing
        memcpy(&tmpset, &readset, sizeof(readset));
        select(maxfd+1, &tmpset, NULL, NULL, 0);
        // Check new connection
        for(i = 0; i < maxfd; i++){
            if(FD_ISSET(i, &tmpset)){                
                if(i == svr.listen_fd){  //server
                    clilen = sizeof(cliaddr);
                    conn_fd = accept(svr.listen_fd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen);
                    if (conn_fd < 0) {
                        if (errno == EINTR || errno == EAGAIN) continue;  // try again
                        if (errno == ENFILE) {
                            (void) fprintf(stderr, "out of file descriptor table ... (maxconn %d)\n", maxfd);
                            continue;
                        }
                        ERR_EXIT("accept")
                    }
                    FD_SET(conn_fd, &readset);
                    requestP[conn_fd].conn_fd = conn_fd;

                    strcpy(requestP[conn_fd].host, inet_ntoa(cliaddr.sin_addr));
                    fprintf(stderr, "getting a new request... fd %d from %s\n", conn_fd, requestP[conn_fd].host);
                }

                else{     //client
                    conn_fd = i;
                    ret = handle_read(&requestP[conn_fd]); // parse data from client to requestP[conn_fd].buf
                    if (ret < 0) {
                        fprintf(stderr, "bad request from %s\n", requestP[conn_fd].host);
                        continue;
                    }       

                    Account member;

                    #ifdef READ_SERVER  //READ_SERVER
                    int id_num = atoi(requestP[conn_fd].buf);
                    if(id_num <= 0 || id_num > 20){
                        sprintf(buf,"invalid id\n");
                        write(requestP[conn_fd].conn_fd, buf ,strlen(buf));
                    }
                    else if(lock_status[id_num-1] == 1 || read_lock( file_fd, (id_num-1)*sizeof(Account), SEEK_SET, sizeof(Account)) == -1){ //locked
                        sprintf(buf,"This account is locked.\n");
                        write(requestP[conn_fd].conn_fd, buf ,strlen(buf));
                    }
                    else{
                        lseek(file_fd, (id_num-1)*sizeof(Account), SEEK_SET);
                        read(file_fd, &member, sizeof(Account));
                        sprintf(buf, "%d %d\n",member.id, member.balance);
                        write(requestP[conn_fd].conn_fd, buf, strlen(buf));
                        unlock(file_fd, (id_num-1)*sizeof(Account), SEEK_SET, sizeof(Account));
                        lock_status[id_num-1] = 0;
                    }
                    FD_CLR(conn_fd, &readset);

                    #else //WRITE_SERVER
                    if(requestP[conn_fd].wait_for_write==0){      //find client
                        requestP[conn_fd].item = atoi(requestP[conn_fd].buf);
                        int id_num = requestP[conn_fd].item;
                        if(id_num <= 0 || id_num > 20){
                            sprintf(buf,"invalid id\n",id_num);
                            write(requestP[conn_fd].conn_fd, buf ,strlen(buf));
                        }
                        else if(lock_status[id_num-1] == 1 || write_lock( file_fd, (id_num-1)*sizeof(Account), SEEK_SET, sizeof(Account)) == -1){ //locked
                            sprintf(buf,"This account is locked.\n");
                            write(requestP[conn_fd].conn_fd, buf ,strlen(buf));
                        }
                       
                        else{
                            sprintf(buf, "This account is modifiable\n");
                            write(requestP[conn_fd].conn_fd, buf, strlen(buf));
                            requestP[conn_fd].wait_for_write = requestP[conn_fd].item;
                            write_lock(file_fd, (id_num-1)*sizeof(Account), SEEK_SET, sizeof(Account));
                            lock_status[id_num-1] = 1;
                            continue;
                        }
                    }
                    else{      //operations
                        int id_num = requestP[conn_fd].item;
                        lseek(file_fd, (id_num-1)*sizeof(Account), SEEK_SET);
                        read(file_fd, &member, sizeof(Account));            
                        char cmd[15];    
                        for(i = 0; i < strlen(requestP[conn_fd].buf); i++) {
                            if (requestP[conn_fd].buf[i] == ' ') {
                                cmd[i] = '\0';
                                break;
                            }
                            cmd[i] = requestP[conn_fd].buf[i];
                        }
                        if(id_num <= 0 || id_num > 20){
                            sprintf(buf,"invalid id\n");
                            write(requestP[conn_fd].conn_fd, buf ,strlen(buf));
                        }
                        else if(strcmp(cmd, "save") == 0){  //save
                            int money = atoi(&requestP[conn_fd].buf[i+1]);
                            if(money >= 0){
                                member.balance += money;
                                lseek(file_fd, (id_num - 1) * sizeof(Account), SEEK_SET);
                                write(file_fd, &member, sizeof(Account));
                            }
                            else{
                                sprintf(buf,"Operation failed.\n");
                                write(requestP[conn_fd].conn_fd, buf ,strlen(buf));
                            }
                        }
                        else if(strcmp(cmd,"withdraw") == 0){ //withdraw
                            int money = atoi(&requestP[conn_fd].buf[i+1]);
                            if(money <= member.balance && money >= 0){
                                member.balance -= money;
                                lseek(file_fd, (id_num - 1) * sizeof(Account), SEEK_SET);
                                write(file_fd, &member, sizeof(Account));
                            }
                            else{
                                sprintf(buf,"Operation failed.\n");
                                write(requestP[conn_fd].conn_fd, buf ,strlen(buf));    
                            }                        
                        }
                        else if(strcmp(cmd, "transfer") == 0){ //transfer
                            char transfer_id[10];
                            for (j = i+1; j < strlen(requestP[conn_fd].buf); j++) {
                                if (requestP[conn_fd].buf[j] == ' ') {
                                    transfer_id[j-i-1] = '\0';
                                    break;
                                }
                                transfer_id[j-i-1] = requestP[conn_fd].buf[j];
                            }
                            int given = atoi(transfer_id);
                            int money = atoi(&requestP[conn_fd].buf[j+1]);
                            if(money >= 0 && money <= member.balance && given <= 20 && given > 0){
                                member.balance -= money;
                                lseek(file_fd, (id_num - 1) * sizeof(Account), SEEK_SET);
                                write(file_fd, &member, sizeof(Account));
                                lseek(file_fd, (given - 1)*sizeof(Account), SEEK_SET);
                                read(file_fd, &member, sizeof(Account));
                                member.balance += money;
                                lseek(file_fd, (given - 1) * sizeof(Account), SEEK_SET);
                                write(file_fd, &member, sizeof(Account));
                            }
                            else{
                                sprintf(buf,"Operation failed.\n");
                                write(requestP[conn_fd].conn_fd, buf ,strlen(buf));   
                            }                                
                        }
                        else if(strcmp(cmd, "balance") == 0){ //balance
                            int money = atoi(&requestP[conn_fd].buf[i+1]);
                            if(money >= 0){
                                 member.balance = money;
                                lseek(file_fd, (id_num - 1) * sizeof(Account), SEEK_SET);
                                write(file_fd, &member, sizeof(Account));
                            }
                            else{
                                sprintf(buf,"Operation failed.\n");
                                write(requestP[conn_fd].conn_fd, buf ,strlen(buf));  
                            }
                        }    
                        else{
                            sprintf(buf,"Operation failed.\n");
                            write(requestP[conn_fd].conn_fd, buf ,strlen(buf));   
                        }
                        unlock(file_fd, (id_num-1)*sizeof(Account), SEEK_SET, sizeof(Account));
                        lock_status[id_num-1] = 0;
                        //    sprintf(buf,"unlock in the end.\n");
                        //    write(requestP[conn_fd].conn_fd, buf ,strlen(buf));  
                    }
                        FD_CLR(conn_fd, &readset);
                        #endif
                        close(requestP[conn_fd].conn_fd);
                        free_request(&requestP[conn_fd]);
                }
            }
        }
    }
    close(file_fd);
    free(requestP);
    return 0;
}


// ======================================================================================================
// You don't need to know how the following codes are working
#include <fcntl.h>

static void* e_malloc(size_t size);


static void init_request(request* reqP) {
    reqP->conn_fd = -1;
    reqP->buf_len = 0;
    reqP->item = 0;
    reqP->wait_for_write = 0;
}

static void free_request(request* reqP) {
    /*if (reqP->filename != NULL) {
        free(reqP->filename);
        reqP->filename = NULL;
    }*/
    init_request(reqP);
}

// return 0: socket ended, request done.
// return 1: success, message (without header) got this time is in reqP->buf with reqP->buf_len bytes. read more until got <= 0.
// It's guaranteed that the header would be correctly set after the first read.
// error code:
// -1: client connection error
static int handle_read(request* reqP) {
    int r;
    char buf[512];

    // Read in request from client
    r = read(reqP->conn_fd, buf, sizeof(buf));
    if (r < 0) return -1;
    if (r == 0) return 0;
    char* p1 = strstr(buf, "\015\012");
    int newline_len = 2;
    // be careful that in Windows, line ends with \015\012
    if (p1 == NULL) {
        p1 = strstr(buf, "\012");
        newline_len = 1;
        if (p1 == NULL) {
            ERR_EXIT("this really should not happen...");
        }
    }
    size_t len = p1 - buf + 1;
    memmove(reqP->buf, buf, len);
    reqP->buf[len - 1] = '\0';
    reqP->buf_len = len-1;
    return 1;
}

static void init_server(unsigned short port) {
    struct sockaddr_in servaddr;
    int tmp;

    gethostname(svr.hostname, sizeof(svr.hostname));
    svr.port = port;

    svr.listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (svr.listen_fd < 0) ERR_EXIT("socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    tmp = 1;
    if (setsockopt(svr.listen_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) {
        ERR_EXIT("setsockopt");
    }
    if (bind(svr.listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        ERR_EXIT("bind");
    }
    if (listen(svr.listen_fd, 1024) < 0) {
        ERR_EXIT("listen");
    }
}

static void* e_malloc(size_t size) {
    void* ptr;

    ptr = malloc(size);
    if (ptr == NULL) ERR_EXIT("out of memory");
    return ptr;
}