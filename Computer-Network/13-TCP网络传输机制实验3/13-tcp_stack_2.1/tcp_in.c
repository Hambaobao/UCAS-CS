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


// clear all the packet that seq_num less than ack_num
// which means thoese packet has been received
static inline void delete_arrived_packet(struct tcp_sock *tsk, int ack_num)
{
	pthread_mutex_lock(&snd_buf_lock);
	if(!list_empty(&tsk->send_buf))
	{
		struct send_buf_entry *entry, *q;
		list_for_each_entry_safe(entry, q, &tsk->send_buf, list)
		{
			struct tcphdr *tcph = packet_to_tcp_hdr(entry->packet);
			struct iphdr *iph = packet_to_ip_hdr(entry->packet);

			if (ntohl(tcph->seq) < ack_num) 
			{
				printf("**** received ack_num: %d, delete packet %d\n", ack_num, ntohl(tcph->seq));

				tsk->snd_wnd += (ntohs(iph->tot_len) - IP_HDR_SIZE(iph) - TCP_HDR_SIZE(tcph));
				list_delete_entry(&entry->list);
				free(entry->packet);
				free(entry);			
			}
		}
	}
	pthread_mutex_unlock(&snd_buf_lock);
}


static inline void add_packet_to_ofo(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	struct rcv_ofo_entry *new_entry = (struct rcv_ofo_entry *)malloc(sizeof(struct rcv_ofo_entry));
	new_entry->packet = (char *)malloc(cb->pl_len);
	memcpy(new_entry->packet, cb->payload, cb->pl_len);
	new_entry->len = cb->pl_len;
	new_entry->seq_num = cb->seq;
	new_entry->seq_end = cb->seq_end;
	new_entry->ack = cb->ack;

	if(list_empty(&tsk->rcv_ofo_buf))
	{
		// printf("****\n insert packet %d\n****\n", new_entry->seq_num);		
		list_insert(&new_entry->list, &tsk->rcv_ofo_buf, tsk->rcv_ofo_buf.next);
		return ;		
	}

	int flag = 1;
	struct rcv_ofo_entry *entry, *q;
	list_for_each_entry_safe(entry, q, &tsk->rcv_ofo_buf, list)
	{
		// printf("****\n entry_seq_num: %d, new_entry_seq_num: %d\n****\n", entry->seq_num, new_entry->seq_num);
		if (entry->seq_num < new_entry->seq_num)
			continue;
		else if (entry->seq_num == new_entry->seq_num)
		{
			free(new_entry->packet);
			free(new_entry);
			return ;
		}
		else
		{
			flag = 0;
			list_insert(&new_entry->list, entry->list.prev, &entry->list);
			// printf("****\n insert packet %d before packet %d\n****\n", new_entry->seq_num, entry->seq_num);
			return ;
		}
	}

	if(flag)
	{
		list_add_tail(&new_entry->list, &tsk->rcv_ofo_buf);
		// printf("****\n insert packet %d after packet %d\n****\n", new_entry->seq_num, entry->seq_num);		
	}
}


u32 global_seq_end = 0;
int last_seq_flag = 0;
static inline void move_ofo_packet_to_ring_buff(struct tcp_sock *tsk)
{
	struct rcv_ofo_entry *entry, *q;
	list_for_each_entry_safe(entry, q, &tsk->rcv_ofo_buf, list)
	{
		if(last_seq_flag)
		{
			if(entry->seq_num == global_seq_end)
			{
				pthread_mutex_lock(&tsk->rcv_buf->lock);
				write_ring_buffer(tsk->rcv_buf, entry->packet, entry->len);
				pthread_mutex_unlock(&tsk->rcv_buf->lock);

				tsk->rcv_nxt = entry->seq_end;
				tsk->snd_una = entry->ack;

				global_seq_end = entry->seq_end;

				list_delete_entry(&entry->list);
				free(entry->packet);
				free(entry);				
			}
			else
			{
				printf("**** Miss seq_num: %u\n", global_seq_end);
				break;
			} 
		}
		else
		{
			pthread_mutex_lock(&tsk->rcv_buf->lock);
			write_ring_buffer(tsk->rcv_buf, entry->packet, entry->len);
			pthread_mutex_unlock(&tsk->rcv_buf->lock);

			tsk->rcv_nxt = entry->seq_end;
			tsk->snd_una = entry->ack;

			global_seq_end = entry->seq_end;
			last_seq_flag = 1;

			list_delete_entry(&entry->list);
			free(entry->packet);
			free(entry);
		}

		printf("**** seq_end: %u\n", global_seq_end);		
	}
}

void do_tcp_state_closed(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	return ;
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
		tcp_unset_retrans_timer(tsk, cb->ack);
		delete_arrived_packet(tsk, cb->ack);

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
		tcp_unset_retrans_timer(tsk, cb->ack);
		delete_arrived_packet(tsk, cb->ack);

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
		tcp_unset_retrans_timer(tsk, cb->ack);
		delete_arrived_packet(tsk, cb->ack);
		add_packet_to_ofo(tsk, cb);
		move_ofo_packet_to_ring_buff(tsk);

		// tsk->rcv_nxt = cb->seq_end;
		// tsk->snd_una = cb->ack;
		
		// pthread_mutex_lock(&tsk->rcv_buf->lock);
		// write_ring_buffer(tsk->rcv_buf, cb->payload, cb->pl_len);
		// pthread_mutex_unlock(&tsk->rcv_buf->lock);

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
		tcp_unset_retrans_timer(tsk, cb->ack);
		delete_arrived_packet(tsk, cb->ack);
	}
}

void do_tcp_state_fin_wait_1(struct tcp_sock *tsk, struct tcp_cb *cb)
{

	if((cb->flags & TCP_ACK))
	{
		tcp_unset_retrans_timer(tsk, cb->ack);
		delete_arrived_packet(tsk, cb->ack);

		tcp_send_control_packet(tsk, TCP_ACK);	
		tcp_set_state(tsk, TCP_FIN_WAIT_2);		
	}

	if(cb->flags & TCP_FIN)
	{
		tcp_unset_retrans_timer(tsk, cb->ack);
		delete_arrived_packet(tsk, cb->ack);

		tcp_send_control_packet(tsk, TCP_ACK);	
		tcp_set_state(tsk, TCP_TIME_WAIT);
		tcp_set_timewait_timer(tsk);		
	}			
}

void do_tcp_state_fin_wait_2(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	if(cb->flags & (TCP_FIN | TCP_ACK))
	{
		tcp_unset_retrans_timer(tsk, cb->ack);
		delete_arrived_packet(tsk, cb->ack);

		tcp_send_control_packet(tsk, TCP_ACK);	
		tcp_set_state(tsk, TCP_TIME_WAIT);
		tcp_set_timewait_timer(tsk);		
	}	
}

void do_tcp_state_close_wait(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	// could be delete ?
	if(cb->flags & (TCP_FIN | TCP_ACK))
	{
		tcp_unset_retrans_timer(tsk, cb->ack);
		delete_arrived_packet(tsk, cb->ack);

		tcp_send_control_packet(tsk, TCP_ACK | TCP_FIN);		
		tcp_set_state(tsk, TCP_LAST_ACK);
	}
}

void do_tcp_state_time_wait(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	return ;
}

void do_tcp_state_last_ack(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	if(cb->flags & TCP_ACK)
	{
		tcp_unset_retrans_timer(tsk, cb->ack);
		delete_arrived_packet(tsk, cb->ack);

		tcp_set_state(tsk, TCP_CLOSED);	
	}
}

char *TCP_State[11] = {"TCP_CLOSED", "TCP_LISTEN", "TCP_SYN_RECV", "TCP_SYN_SENT", "TCP_ESTABLISHED", "TCP_CLOSE_WAIT", 
					"TCP_LAST_ACK", "TCP_FIN_WAIT_1", "TCP_FIN_WAIT_2", "TCP_CLOSING", "TCP_TIME_WAIT"};

// Process the incoming packet according to TCP state machine. 
void tcp_process(struct tcp_sock *tsk, struct tcp_cb *cb, char *packet)
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);

	// printf("DEBUG: State = %s\n", TCP_State[tsk->state]);
	// printf("DEBUG: Received TCP Packet, flags = 0x%x\n", cb->flags);
	// printf("<<<< Seq_num: %d, ACK_num: %d, Seq_end: %d\n", cb->seq, cb->ack, cb->seq_end);


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

	case TCP_TIME_WAIT:
		do_tcp_state_time_wait(tsk, cb);
		break;

	case TCP_LAST_ACK:
		do_tcp_state_last_ack(tsk, cb);
		break;	

	default:
		break;
	}
}
