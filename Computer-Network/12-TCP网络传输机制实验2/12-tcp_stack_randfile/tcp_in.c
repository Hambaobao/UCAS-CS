#include "tcp.h"
#include "tcp_sock.h"
#include "tcp_timer.h"

#include "log.h"
#include "ring_buffer.h"

#include <stdlib.h>
// update the snd_wnd of tcp_sock
//
// if the snd_wnd before updating is zero, notify tcp_sock_send (wait_send)
static inline void tcp_update_window(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	u16 old_snd_wnd = tsk->snd_wnd;
	tsk->snd_wnd = cb->rwnd;
	if (old_snd_wnd == 0)
		wake_up(tsk->wait_send);
}

// update the snd_wnd safely: cb->ack should be between snd_una and snd_nxt
static inline void tcp_update_window_safe(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	if (less_or_equal_32b(tsk->snd_una, cb->ack) && less_or_equal_32b(cb->ack, tsk->snd_nxt))
		tcp_update_window(tsk, cb);
}

#ifndef max
#	define max(x,y) ((x)>(y) ? (x) : (y))
#endif

// check whether the sequence number of the incoming packet is in the receiving
// window
static inline int is_tcp_seq_valid(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	u32 rcv_end = tsk->rcv_nxt + max(tsk->rcv_wnd, 1);
	if (less_than_32b(cb->seq, rcv_end) && less_or_equal_32b(tsk->rcv_nxt, cb->seq_end)) {
		return 1;
	}
	else {
		log(ERROR, "received packet with invalid seq, drop it.");
		return 0;
	}
}

void do_tcp_state_closed(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	tcp_send_reset(cb);
}

void do_tcp_state_listen(struct tcp_sock *tsk, struct tcp_cb *cb)
{	
	if(cb->flags & TCP_SYN) 
	{
		struct tcp_sock *csk = alloc_tcp_sock();
		csk->sk_sip = cb->daddr;
		csk->sk_sport = cb->dport;
		csk->sk_dip = cb->saddr;
		csk->sk_dport = cb->sport;

		csk->parent = tsk;
		csk->rcv_nxt = cb->seq_end;
		csk->iss = tcp_new_iss();
		csk->snd_nxt = csk->iss;
		csk->state = TCP_LISTEN;

		struct sock_addr skaddr = {htonl(csk->sk_sip), htons(csk->sk_sport)};
		tcp_sock_bind(csk, &skaddr);
		
		list_add_tail(&csk->list, &tsk->listen_queue);
		tcp_send_control_packet(csk, TCP_ACK | TCP_SYN);
		tcp_set_state(csk, TCP_SYN_RECV);
		tcp_hash(csk);
	}
	else
		tcp_send_reset(cb);
}

void do_tcp_state_syn_sent(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	if(cb->flags & (TCP_SYN | TCP_ACK)) 
	{
		tsk->rcv_nxt = cb->seq_end;
		tsk->snd_una = cb->ack;
		tcp_send_control_packet(tsk, TCP_ACK);
		tcp_set_state(tsk, TCP_ESTABLISHED);
		wake_up(tsk->wait_connect);
	}
	else
		tcp_send_reset(cb);
}

void do_tcp_state_syn_recv(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	if((cb->flags == TCP_ACK) && (cb->ack == tsk->snd_nxt)) 
	{
		tcp_sock_accept_enqueue(tsk);
		wake_up(tsk->parent->wait_accept);
	}
	else
		tcp_send_reset(cb);
}

void do_tcp_state_established(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	if(cb->flags == (TCP_PSH | TCP_ACK))
	{
		// printf("<<<< Received TCP_PSH | TCP_ACK, flags = 0x%x\n", cb->flags);

		pthread_mutex_lock(&tsk->rcv_buf->lock);
		write_ring_buffer(tsk->rcv_buf, cb->payload, cb->pl_len);
		pthread_mutex_unlock(&tsk->rcv_buf->lock);
		// tsk->rcv_wnd -= cb->pl_len;

		// wake_up(tsk->wait_recv);

		tcp_send_control_packet(tsk, TCP_ACK);	
		return ;		
	}

	if(cb->flags == (TCP_FIN | TCP_ACK))
	{
		// printf("<<<< Received TCP_FIN | TCP_ACK, flags = 0x%x\n", cb->flags);
		tcp_send_control_packet(tsk, TCP_FIN | TCP_ACK);
		tcp_set_state(tsk, TCP_CLOSE_WAIT);
		tcp_set_state(tsk, TCP_LAST_ACK);
	
		return ;
	}

	if(cb->flags == TCP_FIN)
	{
		// printf("<<<< Received TCP_FIN, flags = 0x%x\n", cb->flags);
		tcp_send_control_packet(tsk, TCP_ACK);	
		tcp_set_state(tsk, TCP_CLOSE_WAIT);
	}

	if(cb->flags == TCP_ACK)
	{
		// wake_up(tsk->wait_send);
	}

}

void do_tcp_state_fin_wait_1(struct tcp_sock *tsk, struct tcp_cb *cb)
{

	if((cb->flags & TCP_ACK))
	{
		tcp_send_control_packet(tsk, TCP_ACK);	
		tcp_set_state(tsk, TCP_FIN_WAIT_2);		
	}

	if(cb->flags & TCP_FIN)
	{
		tcp_send_control_packet(tsk, TCP_ACK);	
		tcp_set_state(tsk, TCP_TIME_WAIT);
		tcp_set_timewait_timer(tsk);		
	}			
}

void do_tcp_state_fin_wait_2(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	if(cb->flags & (TCP_FIN | TCP_ACK))
	{
		tcp_send_control_packet(tsk, TCP_ACK);	
		tcp_set_state(tsk, TCP_TIME_WAIT);
		tcp_set_timewait_timer(tsk);		
	}	
}

void do_tcp_state_close_wait(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	if(cb->flags & (TCP_FIN | TCP_ACK))
	{
		tcp_send_control_packet(tsk, TCP_ACK | TCP_FIN);		
		tcp_set_state(tsk, TCP_LAST_ACK);
	}
}

void do_tcp_state_last_ack(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	if(cb->flags & TCP_ACK)
		tcp_set_state(tsk, TCP_CLOSED);	
}

char *TCP_State[11] = {"TCP_CLOSED", "TCP_LISTEN", "TCP_SYN_RECV", "TCP_SYN_SENT", "TCP_ESTABLISHED", "TCP_CLOSE_WAIT", 
					"TCP_LAST_ACK", "TCP_FIN_WAIT_1", "TCP_FIN_WAIT_2", "TCP_CLOSING", "TCP_TIME_WAIT"};

// Process the incoming packet according to TCP state machine. 
void tcp_process(struct tcp_sock *tsk, struct tcp_cb *cb, char *packet)
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);

	// printf("<<<< State = %s\n", TCP_State[tsk->state]);
	// printf("<<<< Received TCP Packet, flags = 0x%x\n", cb->flags);

	tsk->snd_una = cb->ack;
	tsk->rcv_nxt = cb->seq_end;

	switch (tsk->state)
	{
	case TCP_CLOSED:
		do_tcp_state_closed(tsk, cb);
		break;
	
	case TCP_LISTEN:
		do_tcp_state_listen(tsk, cb);
		break;

	case TCP_SYN_SENT:
		do_tcp_state_syn_sent(tsk, cb);
		break;

	case TCP_SYN_RECV:
		do_tcp_state_syn_recv(tsk, cb);
		break;

	case TCP_FIN_WAIT_1:
		do_tcp_state_fin_wait_1(tsk, cb);
		break;

	case TCP_FIN_WAIT_2:
		do_tcp_state_fin_wait_2(tsk, cb);
		break;

	case TCP_ESTABLISHED:
		do_tcp_state_established(tsk, cb);
		break;

	case TCP_CLOSE_WAIT:
		do_tcp_state_close_wait(tsk, cb);
		break;

	case TCP_LAST_ACK:
		do_tcp_state_last_ack(tsk, cb);
		break;	

	default:
		break;
	}



/*
	int state = tsk->state;

	if(state == TCP_CLOSED)
	{
		do_tcp_state_closed(tsk, cb);
		return ;		
	}

	if(state == TCP_LISTEN)
	{
		do_tcp_state_listen(tsk, cb);
		return ;
	}

	if(state == TCP_SYN_SENT)
	{
		do_tcp_state_syn_sent(tsk, cb);
		return ;
	}

	tsk->snd_una = cb->ack;
	tsk->rcv_nxt = cb->seq_end;

	if(state == TCP_SYN_RECV)
	{
		do_tcp_state_syn_recv(tsk, cb);
		return ;
	}

	if(state == TCP_FIN_WAIT_1)
	{
		do_tcp_state_fin_wait_1(tsk, cb);
		return ;
	}

	if(state == TCP_FIN_WAIT_2)
	{
		do_tcp_state_fin_wait_2(tsk, cb);
		return ;
	}

	if(state == TCP_ESTABLISHED)
	{
		do_tcp_state_established(tsk, cb);
		return;
	}

	if(state == TCP_CLOSE_WAIT)
	{
		do_tcp_state_close_wait(tsk, cb);
		return ;
	}

	if(state == TCP_LAST_ACK)
	{
		do_tcp_state_last_ack(tsk, cb);
		return ;
	}
*/
}
