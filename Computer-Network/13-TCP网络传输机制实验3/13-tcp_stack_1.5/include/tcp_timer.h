#ifndef __TCP_TIMER_H__
#define __TCP_TIMER_H__

#include "list.h"

#include <stddef.h>

pthread_mutex_t ip_send_lock;
pthread_mutex_t snd_buf_lock;

pthread_mutex_t retrans_lock;
pthread_mutex_t timewait_lock;

struct list_head timewait_list;
struct list_head retrans_list;

struct tcp_timer {
	int type;	// time-wait: 0		retrans: 1
	int timeout;	// in micro second
	struct list_head list;
	int enable;
	struct tcp_sock *tsk;	
};

struct tcp_retrans_timer {
	int type;	// time-wait: 0		retrans: 1
	int timeout;	// in micro second
	struct list_head list;
	int retrans_count;
	struct tcp_sock *tsk;
	int seq_num;
};

struct tcp_sock;
#define timewait_to_tcp_sock(t) \
	(struct tcp_sock *)((char *)(t) - offsetof(struct tcp_sock, timewait))

#define retranstimer_to_tcp_sock(t) \
	(struct tcp_sock *)((char *)(t) - offsetof(struct tcp_sock, retrans_timer))
#define TCP_TIMER_SCAN_INTERVAL 100000
#define TCP_MSL			1000000
#define TCP_TIMEWAIT_TIMEOUT	(2 * TCP_MSL)
#define TCP_RETRANS_INTERVAL_INITIAL 200000


// the thread that scans timer_list periodically
void *tcp_timer_thread(void *arg);
// add the timer of tcp sock to timer_list
void tcp_set_timewait_timer(struct tcp_sock *);

void tcp_set_retrans_timer(struct tcp_sock *tsk, int seq_num);

void tcp_unset_retrans_timer(struct tcp_sock *tsk, int ack_num);

#endif
