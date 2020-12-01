#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

#define FILENOTFOUND "HTTP/1.1 404 file not found\r\n\r\n"
#define FILEFOUND "HTTP/1.1 200 ok\r\n"
#define THREAD_NUM 8

int file_size(char* filename)
{
    FILE *fp=fopen(filename,"r");
    if(!fp) return -1;
    fseek(fp,0L,SEEK_END);
    int size=ftell(fp);
    fclose(fp);
    
    return size;
}

int handle_request(int csockfd) 
{
    printf("%d: connection accepted\n", csockfd);

    FILE* fd;
    char msg[2000];
    char file_path[200];
    msg[0] = 0;
    file_path[0] = 0;

    int msg_len = 0;
    // receive a message from client
    while ((msg_len = recv(csockfd, msg, sizeof(msg), 0)) > 0) {
        msg[msg_len] = 0;
        printf("%s\n", msg);

        char* p[20]; 
        p[0] = strtok(msg, " ");
        int i = 1;
        while((p[i] = strtok(NULL, " ")) != NULL) {
            i++;
        }
        strcat(file_path, ".");
        strcat(file_path, p[1]);

        // printf("file_path: %s\n", file_path);
        if((fd = fopen(file_path, "r")) == NULL) {
            printf("file dosen't exist\n");
            write(csockfd, FILENOTFOUND, strlen(FILENOTFOUND));
            return 1;              
        }
        int fsize = file_size(file_path);
        char fsize_str[20];
        sprintf(fsize_str, "%d", fsize);

        msg[0] = 0;
        strcat(msg, FILEFOUND);
        strcat(msg, "Content-Length: ");
        strcat(msg, fsize_str);
        strcat(msg, "\r\n");
        strcat(msg, "Content-Type: text/html; charset=utf-8");
        strcat(msg, "\r\n\r\n");
        write(csockfd, msg, strlen(msg));

        // send data
        while (1)
        {
            memset(msg,0,sizeof(msg));       //缓存清0
            fgets(msg, sizeof(msg), fd);
            if (send(csockfd, msg, strlen(msg), 0) < 0)
            {
                printf("send Failed");
                return 1;
            }
            if(feof(fd))
                break;
        }

        fclose(fd);
    }  

    if (msg_len == 0) {
        printf("client disconnected\n");
    }
    else { // msg_len < 0
        perror("recv failed\n");
		return -1;
    }

    printf("%d: request finished\n", csockfd);

    return 0;
}

int main(int argc, const char *argv[])
{

    int sockfd;
    struct sockaddr_in server, client;

    // create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("create socket failed\n");
        return -1;
    }
    printf("socket created\n"); 

    // prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(80);       

    // bind
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind failed\n");
        return -1;
    }
    printf("bind done\n");

    // listen
    listen(sockfd, 1024);

    while(1) {
        int c = sizeof(struct sockaddr_in);
        printf("waiting for incoming connections...\n");
        int request = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&c);
        if(request < 0)
            continue;

        pthread_t *tid;
        tid = (pthread_t *)malloc(sizeof(pthread_t));
        pthread_create(tid, NULL, handle_request, (void *)request);
        sleep(1);
    }

    return 0;            
}