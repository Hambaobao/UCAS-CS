#include "arpcache.h"
#include "arp.h"
#include "ether.h"
#include "packet.h"
#include "icmp.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

static arpcache_t arpcache;


// initialize IP->mac mapping, request list, lock and sweeping thread
void arpcache_init()
{
	bzero(&arpcache, sizeof(arpcache_t));

	init_list_head(&(arpcache.req_list));

	pthread_mutex_init(&arpcache.lock, NULL);

	pthread_create(&arpcache.thread, NULL, arpcache_sweep, NULL);
}

// release all the resources when exiting
void arpcache_destroy()
{
	pthread_mutex_lock(&arpcache.lock);

	struct arp_req *req_entry = NULL, *req_q;
	list_for_each_entry_safe(req_entry, req_q, &(arpcache.req_list), list) {
		struct cached_pkt *pkt_entry = NULL, *pkt_q;
		list_for_each_entry_safe(pkt_entry, pkt_q, &(req_entry->cached_packets), list) {
			list_delete_entry(&(pkt_entry->list));
			free(pkt_entry->packet);
			free(pkt_entry);
		}

		list_delete_entry(&(req_entry->list));
		free(req_entry);
	}

	pthread_kill(arpcache.thread, SIGTERM);

	pthread_mutex_unlock(&arpcache.lock);
}

// lookup the IP->mac mapping
//
// traverse the table to find whether there is an entry with the same IP
// and mac address with the given arguments
int arpcache_lookup(u32 ip4, u8 mac[ETH_ALEN])
{
	pthread_mutex_lock(&arpcache.lock);
	for(int i = 0; i < MAX_ARP_SIZE; i++) {
		if(arpcache.entries[i].ip4 == ip4 && arpcache.entries[i].valid) 
		{
			memcpy(mac, arpcache.entries[i].mac, ETH_ALEN);
			pthread_mutex_unlock(&arpcache.lock);	
			return 1;
		}
	}
	pthread_mutex_unlock(&arpcache.lock);	
	return 0;
}

// append the packet to arpcache
//
// Lookup in the list which stores pending packets, if there is already an
// entry with the same IP address and iface (which means the corresponding arp
// request has been sent out), just append this packet at the tail of that entry
// (the entry may contain more than one packet); otherwise, malloc a new entry
// with the given IP address and iface, append the packet, and send arp request.
void arpcache_append_packet(iface_info_t *iface, u32 ip4, char *packet, int len)
{
	// fprintf(stderr, "TODO: append the ip address if lookup failed, and send arp request if necessary.\n");
	
	pthread_mutex_lock(&arpcache.lock);

	// printf(">>>> Appending Packet in Arpcache, DST_IP of Packet==0x%x\n", ip4);

	int cached = 0;
	struct arp_req *req_entry = NULL, *req_q;
	// 如果已经有相同的IP地址项
	list_for_each_entry_safe(req_entry, req_q, &(arpcache.req_list), list) {
		if(req_entry->ip4 == ip4) 
		{
			struct cached_pkt *new_pkt_entry = (struct cached_pkt *)malloc(sizeof(struct cached_pkt));
			new_pkt_entry->packet = packet;
			new_pkt_entry->len = len;
			list_add_tail(&new_pkt_entry->list, &req_entry->cached_packets);
			cached = 1;
		}
	}
	// 如果没有相同IP地址项
	if(cached == 0)
	{
		// 新分配一个项
		struct arp_req *req_entry = (struct arp_req *)malloc(sizeof(struct arp_req));
		init_list_head(&req_entry->cached_packets);
		req_entry->iface = iface;
		req_entry->ip4 = ip4;
		req_entry->sent = time(NULL);
		req_entry->retries = 0;
		list_add_tail(&req_entry->list, &(arpcache.req_list));
		
		// 把数据包加入缓存
		struct cached_pkt *new_pkt_entry = (struct cached_pkt *)malloc(sizeof(struct cached_pkt));
		new_pkt_entry->packet = packet;
		new_pkt_entry->len = len;
		init_list_head(&new_pkt_entry->list);
		list_add_tail(&new_pkt_entry->list, &req_entry->cached_packets);		
	}	

	pthread_mutex_unlock(&arpcache.lock);

	// printf(">>>> Appending Packet Succeed, DST_IP of Packet==0x%x\n", ip4);

	arp_send_request(iface, ip4);		
}

// insert the IP->mac mapping into arpcache, if there are pending packets
// waiting for this mapping, fill the ethernet header for each of them, and send
// them out
void arpcache_insert(u32 ip4, u8 mac[ETH_ALEN])
{
	/*fprintf(stderr, "TODO: insert ip->mac entry, and send all the pending packets.\n");*/
	pthread_mutex_lock(&arpcache.lock);

	// printf(">>>> Arpcache inserting. Received IP==0x%x\n", ip4);

	// 将该IP->MAC映射填入表中
	int i;
	for(i = 0; i < MAX_ARP_SIZE; i++)
		if(arpcache.entries[i].valid && arpcache.entries[i].ip4 == ip4)
		{
			memcpy(&(arpcache.entries[i].mac), mac, ETH_ALEN);
			arpcache.entries[i].added = time(NULL);
			break;
		}
	// 如果表中原来没有，则新加入
	if(i == MAX_ARP_SIZE) {
		for(i = 0; i < MAX_ARP_SIZE; i++)
			if(arpcache.entries[i].valid == 0) 
			{
				arpcache.entries[i].ip4 = ip4;
				memcpy(&(arpcache.entries[i].mac), mac, ETH_ALEN);
				arpcache.entries[i].added = time(NULL);
				arpcache.entries[i].valid = 1;
				break;
			}
		// 如果表满了，则随机替换
		if(i == MAX_ARP_SIZE) {
			i = rand() % 32;
			arpcache.entries[i].ip4 = ip4;
			memcpy(arpcache.entries[i].mac, mac, ETH_ALEN);
			arpcache.entries[i].added = time(NULL);
			arpcache.entries[i].valid = 1;
		}	
	}

	// printf(">>>> Insert IP->MAC in Arpcache Succeed.\n");

	// 将在缓存中等待该映射的数据包，依次填写mac地址，转发出去，并删除缓存包
	struct arp_req *req_entry, *req_q;
	list_for_each_entry_safe(req_entry, req_q, &(arpcache.req_list), list) {
		if(req_entry->ip4 == ip4)
		{
			// printf(">>>> Found Entry Waiting For IP->MAC. IP==0x%x\n", ip4);
			struct cached_pkt *pkt_entry, *pkt_q;
			list_for_each_entry_safe(pkt_entry, pkt_q, &(req_entry->cached_packets), list) {
				// 填写目的mac地址
				struct ether_header *eh = (struct ether_header *)(pkt_entry->packet);
				memcpy(eh->ether_dhost, mac, ETH_ALEN);
				eh->ether_type = htons(ETH_P_IP);
				// 转发数据包

				// printf(">>>> Send A Packet From Arpcache.\n");
				iface_send_packet(req_entry->iface, pkt_entry->packet, pkt_entry->len);
			}
			// 删除相应缓存数据包
			list_for_each_entry_safe(pkt_entry, pkt_q, &(req_entry->cached_packets), list) {
				list_delete_entry(&(pkt_entry->list));
				free(pkt_entry);
			}
		}
	}
	pthread_mutex_unlock(&arpcache.lock);
}

// sweep arpcache periodically
//
// For the IP->mac entry, if the entry has been in the table for more than 15
// seconds, remove it from the table.
// For the pending packets, if the arp request is sent out 1 second ago, while 
// the reply has not been received, retransmit the arp request. If the arp
// request has been sent 5 times without receiving arp reply, for each
// pending packet, send icmp packet (DEST_HOST_UNREACHABLE), and drop these
// packets.
void *arpcache_sweep(void *arg) 
{
	while (1) {
		sleep(1);
		pthread_mutex_lock(&arpcache.lock);

		for(int i = 0; i < MAX_ARP_SIZE; i++) {
			if(arpcache.entries[i].valid && time(NULL) - arpcache.entries[i].added > 15) {
				arpcache.entries[i].valid = 0;
			}
		}

		struct arp_req *req_entry, *req_q;
		list_for_each_entry_safe(req_entry, req_q, &(arpcache.req_list), list){
			// 如果arp请求超过1秒，重新发送arp请求
			if(time(NULL) - req_entry->sent > 1 && req_entry->retries <= 5)
			{
				// printf("**** Resent ARP Request %d times.\n", req_entry->retries + 1);

				req_entry->retries++;
				req_entry->sent= time(NULL);
				arp_send_request(req_entry->iface, req_entry->ip4);
			}
			// 如果超多5次未收到arp应答，对该队列下的数据包依次回复icmp不可达，并删除数据包
			else if(req_entry->retries > 5)
			{
				struct cached_pkt *pkt_entry, *pkt_q;
				list_for_each_entry_safe(pkt_entry, pkt_q, &(req_entry->cached_packets), list){
					
					pthread_mutex_unlock(&arpcache.lock);
					// ARP查询失败, type==3, code==1
					// printf("**** Retried more than 5 times, DST_IP==0x%x Unreach\n", req_entry->ip4);
					icmp_send_packet(pkt_entry->packet, pkt_entry->len, ICMP_DEST_UNREACH, ICMP_HOST_UNREACH);
					pthread_mutex_lock(&arpcache.lock);
					
					list_delete_entry(&(pkt_entry->list));
					free(pkt_entry->packet);
					free(pkt_entry);					
				}
				list_delete_entry(&(req_entry->list));
				free(req_entry);
			}
		}	
		pthread_mutex_unlock(&arpcache.lock);
	}
	return NULL;
}
