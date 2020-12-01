/* client application */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

char recv_file_path[200] = {0,};
char file_path[200] = {0,};

int main(int argc, char *argv[])
{
	if(argc != 2) {
		printf("Usage: ./client url\n");
		return 1;	
	}

    char* ip;
    char* file_name;
    int sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000];
    FILE *fd;

    char* p[20]; 
    p[0] = strtok(argv[1], "/");
    int i = 1;
    while((p[i] = strtok(NULL, "/")) != NULL) {
        i++;
    }
    ip = p[0];
    file_name = p[i-1];

    // set file name and path
    strcat(file_path, ".");
    for(int j = 1; j < i; j++) {
        strcat(file_path, "/");
        strcat(file_path, p[j]);        
    }

    strcat(recv_file_path, "./client_recv/");
    strcat(recv_file_path, file_name);

    // printf("ip: %s\n", ip);
    // printf("file_path: %s\n", file_path);
    // printf("%s\n", file_path+2);
    // printf("recv_file_path: %s\n", recv_file_path);

    // create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("create socket failed");
		return -1;
    }
    printf("socket created");
     
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
 
    // connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed");
        return 1;
    }
     
    printf("connected\n");
     
    // construct the http head
    strcat(message, "GET /");
    strcat(message, file_path + 2);
    strcat(message, "  HTTP/1.1\r\n");
    strcat(message, "\r\n");
    printf("http head: %s", message);

    // send some data
    if (send(sock, message, strlen(message), 0) < 0) {
        printf("send failed\n");
        return 1;
    }

    if((fd = fopen(recv_file_path, "w")) == NULL) {
        printf("open file failed\n");
        return 1;        
    };

    unsigned long long record_size = 0;
    unsigned long long file_length = 0;
    printf("server reply : \n");
    while(1) {      
        // receive a reply from the server
        static int count = 1;

		int len = recv(sock, server_reply, 2000, 0);
        if (len <= 0) {
            if(count == 1)
                printf("\nrecv failed");
            break;
        }
        server_reply[len] = 0;
        // printf("%s", server_reply);
        
        // printf("receiving...received: %d, file_size: %d\n", record_size, file_length);
        if(count == 1) {
            char* start;
            for (int index = 0; index < len - 4; index++)
            {
                if(server_reply[index] == '\r' && 
                    server_reply[index + 1] == '\n' && 
                    server_reply[index + 2] == '\r' && 
                    server_reply[index + 3] == '\n'
                    ) 
                {
                    start = server_reply + index + 4;
                    fwrite(start, len - index - 4, 1, fd);
                    record_size += len - index - 4;
                    float radio = record_size * 100 / file_length;
                    printf(">>>receiving: %.2f%%\n", radio);
                    break;                 
                }
                else if(server_reply[index] == 'L' &&
                        server_reply[index + 1] == 'e' &&
                        server_reply[index + 2] == 'n' &&
                        server_reply[index + 3] == 'g' &&
                        server_reply[index + 4] == 't' &&
                        server_reply[index + 5] == 'h' &&
                        server_reply[index + 6] == ':'
                        ) 
                {
                    char length_str[20];
                    int k = 0;
                    for(int j = index + 7; server_reply[j] != '\r' && k < 20; j++,k++)
                        length_str[k] = server_reply[j];
                        length_str[k] = 0;
                    file_length = atoi(length_str);
                }                
            }            
        }
        else {
            fwrite(server_reply, len, 1, fd);
            record_size += len;
            float radio = record_size * 100 / file_length;
            printf(">>>receiving: %.2f%%\n", radio);
        }

        if(record_size == file_length)
            break;

        count++;
    }
    printf("\n\nreceive done\n");
    
    fclose(fd);
    close(sock);

    return 0;
}
