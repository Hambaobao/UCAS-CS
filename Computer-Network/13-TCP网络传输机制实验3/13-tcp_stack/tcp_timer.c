#include "tcp.h"
#include "tcp_timer.h"
#include "tcp_sock.h"

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static struct list_head timewait_list;
static struct list_head retrans_list;

static struct tcp_timer wait_timer;
static struct tcp_retrans_timer retrans_timer;

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

	retrans_timer.timeout -= TCP_TIMER_SCAN_INTERVAL;
	if (retrans_timer.enable == 1 && retrans_timer.timeout <= 0 && retrans_timer.type == 1) 
	{
		struct tcp_sock *tsk = retrans_timer.tsk;			

		if(retrans_timer.retrans_count == 3)
		{
			tcp_sock_close(tsk);
			// free_tcp_sock(tsk);
			
			return ;
		}

		struct send_buf_entry *entry = list_entry(tsk->send_buf.next, struct send_buf_entry, list);

		ip_send_packet(entry->packet, entry->len);
		retrans_timer.timeout = TCP_RETRANS_INTERVAL_INITIAL;

		for(int i = 0; i < retrans_timer.retrans_count; i++)
			retrans_timer.timeout <<= 1;

		retrans_timer.retrans_count++;
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

void tcp_set_retrans_timer(struct tcp_sock *tsk)
{	
	// type == 1: retrans
	pthread_mutex_lock(&retrans_lock);
	retrans_timer.type = 1;
	retrans_timer.timeout = TCP_RETRANS_INTERVAL_INITIAL;
	retrans_timer.retrans_count = 0;
	retrans_timer.enable = 1;
	retrans_timer.tsk = tsk;

	pthread_mutex_unlock(&retrans_lock);	
}

void tcp_unset_retrans_timer(struct tcp_sock *tsk)
{
	pthread_mutex_lock(&retrans_lock);
	retrans_timer.enable = 0;
	pthread_mutex_unlock(&retrans_lock);
}

// scan the timer_list periodically by calling tcp_scan_timer_list
void *tcp_timer_thread(void *arg)
{
	init_list_head(&timewait_list);
	init_list_head(&retrans_list);

	while (1) {
		usleep(TCP_TIMER_SCAN_INTERVAL);
		tcp_scan_timer_list();	
		tcp_scan_retrans_list();			
	}

	return NULL;
}
