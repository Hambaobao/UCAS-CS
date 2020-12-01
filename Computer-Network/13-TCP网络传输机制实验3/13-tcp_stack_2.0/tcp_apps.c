#include "tcp_sock.h"

#include "log.h"

#include <stdlib.h>
#include <unistd.h>

// tcp server application, listens to port (specified by arg) and serves only one
// connection request
void *tcp_server(void *arg)
{
	u16 port = *(u16 *)arg;
	struct tcp_sock *tsk = alloc_tcp_sock();

	struct sock_addr addr;
	addr.ip = htonl(0);
	addr.port = port;
	if (tcp_sock_bind(tsk, &addr) < 0) {
		log(ERROR, "tcp_sock bind to port %hu failed", ntohs(port));
		exit(1);
	}

	if (tcp_sock_listen(tsk, 3) < 0) {
		log(ERROR, "tcp_sock listen failed");
		exit(1);
	}

	log(DEBUG, "listen to port %hu.", ntohs(port));

	struct tcp_sock *csk = tcp_sock_accept(tsk);

	log(DEBUG, "accept a connection.");

	FILE *fp; 								
	if((fp = fopen("server-output.dat", "w")) == NULL) 
	{ 
		printf("Open File Falied!\n"); 
		return NULL; 
	}

	u32 tot_rsize = 0;

	char rbuf[1001];
	int rlen = 0;
	while (1) {
		rlen = tcp_sock_read(csk, rbuf, 1000);
		if (rlen == 0) {
			log(DEBUG, "tcp_sock_read return 0, finish transmission.");
			break;
		} 
		else if (rlen > 0) 
		{
			int ret = fwrite(rbuf, 1, rlen, fp);
			fflush(fp);

			if(ret == 0 || ret < rlen)
			{
				printf("**** Write Error\n");
				break;
			}

			tot_rsize += ret;
			
			// for(int i = 0; i < rlen; i++)
			// 	printf("%c", rbuf[i]);
			// printf("\n");

			printf("**** Totaly Received %u bytes\n", tot_rsize);
		}
		else {
			log(DEBUG, "tcp_sock_read return negative value, something goes wrong.");
			exit(1);
		}

	}

	fclose(fp);

	log(DEBUG, "close this connection.");

	tcp_sock_close(csk);
	
	return NULL;
}

// tcp client application, connects to server (ip:port specified by arg), each
// time sends one bulk of data and receives one bulk of data 
void *tcp_client(void *arg)
{
	struct sock_addr *skaddr = arg;

	struct tcp_sock *tsk = alloc_tcp_sock();

	if (tcp_sock_connect(tsk, skaddr) < 0) {
		log(ERROR, "tcp_sock connect to server ("IP_FMT":%hu)failed.", \
				NET_IP_FMT_STR(skaddr->ip), ntohs(skaddr->port));
		exit(1);
	}

	FILE *fp; 								
	if((fp = fopen("client-input.dat", "r")) == NULL) 
	{ 
		printf("Open File Falied!\n"); 
		return NULL; 
	}

	char wbuf[1001];

	u32 tot_wsize = 0;

	while(!feof(fp)) 
	{
		u32 wlen = fread(wbuf, 1, 1000, fp);	

		if (tcp_sock_write(tsk, wbuf, wlen) < 0)
			break;

		tot_wsize += wlen;
		
		printf("**** Totaly Sent %u bytes\n", tot_wsize);

		// sleep 0.01s
		usleep(10000);
	}	

	sleep(1);

	fclose(fp);

	tcp_sock_close(tsk);

	return NULL;
}
