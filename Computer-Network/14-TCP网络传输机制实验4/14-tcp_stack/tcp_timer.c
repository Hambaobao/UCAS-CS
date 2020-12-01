#include "tcp.h"
#include "tcp_timer.h"
#include "tcp_sock.h"

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

static struct tcp_timer wait_timer;

// scan the timer_list, find the tcp sock which stays for at 2*MSL, release it
void tcp_scan_timer_list()
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);
	pthread_mutex_lock(&timewait_lock);	

	wait_timer.timeout -= TCP_TIMER_SCAN_INTERVAL;
	if (wait_timer.enable == 1 && wait_timer.timeout <= 0 && wait_timer.type == 0) 
	{
		wait_timer.enable = 0;

		struct tcp_sock *tsk = wait_timer.tsk;
		if (!tsk->parent)
			tcp_bind_unhash(tsk);
		tcp_set_state(tsk, TCP_CLOSED);
		// free_tcp_sock(tsk);
	}
	pthread_mutex_unlock(&timewait_lock);		
}

void tcp_scan_retrans_list()
{
	pthread_mutex_lock(&retrans_lock);	

	struct tcp_retrans_timer *timer, *q;
	list_for_each_entry_safe(timer, q, &retrans_list, list)
	{
		timer->timeout -= TCP_TIMER_SCAN_INTERVAL;
		if (timer->timeout <= 0 && timer->type == 1) 
		{
			printf("===================Retrans Timeout===================\n");

			struct tcp_sock *tsk = timer->tsk;			
			if(timer->retrans_count == 3)
			{
				// printf("====================================================\n");
				// printf("====================tcp_sock_close==================\n");
				tcp_sock_close(tsk);
				// free_tcp_sock(tsk);				
				
				break;
			}

			tcp_set_con_state(tsk, CON_LOSS);
			do_tcp_con_state_loss(tsk);

			pthread_mutex_lock(&snd_buf_lock);
			if(!list_empty(&tsk->send_buf))
			{
				struct send_buf_entry *entry;
				list_for_each_entry(entry, &tsk->send_buf, list)
					break;

				struct tcphdr *tcp = packet_to_tcp_hdr(entry->packet);

				printf(">>>> send packet entry->seq_num: %d\n", ntohl(tcp->seq));

				char *packet_out = (char *)malloc(entry->len);
				memcpy(packet_out, entry->packet, entry->len);

				pthread_mutex_lock(&ip_send_lock);	
				ip_send_packet(packet_out, entry->len);
				pthread_mutex_unlock(&ip_send_lock);			
			}

			pthread_mutex_unlock(&snd_buf_lock);

			timer->timeout = TCP_RETRANS_INTERVAL_INITIAL;

			for(int i = 0; i < timer->retrans_count; i++)
				timer->timeout *= 2;

			timer->retrans_count++;

			printf("====================Retrans %d Times==================\n", timer->retrans_count);

			break;
		}
	}

	pthread_mutex_unlock(&retrans_lock);		
}

// set the timewait timer of a tcp sock, by adding the timer into timer_list
void tcp_set_timewait_timer(struct tcp_sock *tsk)
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);

	// type == 0: time_wait
	pthread_mutex_lock(&timewait_lock);		
	wait_timer.type = 0;
	wait_timer.enable = 1;
	wait_timer.timeout = TCP_TIMEWAIT_TIMEOUT;
	wait_timer.tsk = tsk;
	tsk->ref_cnt++;
	pthread_mutex_unlock(&timewait_lock);	
}

void tcp_set_retrans_timer(struct tcp_sock *tsk, int seq_num)
{	
	struct tcp_retrans_timer *timer = (struct tcp_retrans_timer *)malloc(sizeof(struct tcp_retrans_timer));
	timer->type = 1;
	timer->timeout = TCP_RETRANS_INTERVAL_INITIAL;
	timer->retrans_count = 0;
	timer->seq_num = seq_num;
	timer->tsk = tsk;

	pthread_mutex_lock(&retrans_lock);	
	list_add_tail(&timer->list, &retrans_list);
	pthread_mutex_unlock(&retrans_lock);	
}

void tcp_unset_retrans_timer(struct tcp_sock *tsk, int ack_num)
{
	// printf("**** DO TCP UNSET RETRANS\n");
	pthread_mutex_lock(&retrans_lock);

	struct tcp_retrans_timer *timer, *q;
	list_for_each_entry_safe(timer, q, &retrans_list, list)
	{
		// printf("**** Timer Seq_num %d, Ack_num %d\n", timer->seq_num, ack_num);		
		if(timer->seq_num <= ack_num)
		{
			// printf("**** Delete Timer %d\n", timer->seq_num);
			list_delete_entry(&timer->list);
			free(timer);
		}
		else 
			break;
	}

	pthread_mutex_unlock(&retrans_lock);
}

// scan the timer_list periodically by calling tcp_scan_timer_list
void *tcp_timer_thread(void *arg)
{
	while (1) {
		usleep(TCP_TIMER_SCAN_INTERVAL);
		tcp_scan_timer_list();	
		tcp_scan_retrans_list();			
	}

	return NULL;
}
