#include "ip.h"
#include "arp.h"
#include "icmp.h"
#include "rtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void ip_forward_packet(u32 dst, char *packet, int len)
{
	// printf(">>>> Forwarding IP Packet, DST_IP==0x%x.\n", dst);

	struct iphdr *iph = packet_to_ip_hdr(packet);
	// 如果TTL值减为0，发送icmp报文；
	if (--iph->ttl == 0){
		icmp_send_packet(packet, len, ICMP_TIME_EXCEEDED, ICMP_NET_UNREACH);
		return;
	}
	// 查找路由表，如果路由表查找失败，发送icmp报文；如果有default，则一定能找到
	// longest_prefix_match()最长前缀匹配找到相应路由表项
	rt_entry_t *entry = longest_prefix_match(dst);
	iph->checksum = ip_checksum(iph);
	//	得到rt_entry_t里的gw，即下一跳ip地址;


	// fprintf(stderr, "Entry == %x\n", entry);
	if (entry){
		u32 next_hop = entry->gw ? entry->gw : dst;
		// 使用 iface_send_packet_by_arp()发送；
		// printf(">>>> Found Router Entry, Next_Hop==0x%x.\n", next_hop);
		iface_send_packet_by_arp(entry->iface, next_hop, packet, len);
	}
	else {
		// printf(">>>> Find Router Entry Failed.\n");
		icmp_send_packet(packet, len, ICMP_DEST_UNREACH, ICMP_NET_UNREACH);
	}
}

// handle ip packet
//
// If the packet is ICMP echo request and the destination IP address is equal to
// the IP address of the iface, send ICMP echo reply; otherwise, forward the
// packet.
void handle_ip_packet(iface_info_t *iface, char *packet, int len)
{
	/*fprintf(stderr, "TODO: handle ip packet: echo the ping packet, and forward other IP packets.\n");*/
	struct ether_header *eh = (struct ether_header *)packet;
	struct iphdr *iph = packet_to_ip_hdr(packet);
	memcpy(eh->ether_shost, iface->mac, ETH_ALEN);

	// 如果是icmp请求，且目的地址与iface的ip4相等，回应icmp消息；
	if(iph->protocol == IPPROTO_ICMP)
	{
		struct icmphdr * icmp = (struct icmphdr *)(packet + ETHER_HDR_SIZE + IP_HDR_SIZE(iph));
		// 如果收到ping本端口的数据包，发送icmp报文；
		// printf(">>>> Receive ICMP Packet, DST_IP==0x%x, Iface_IP==0x%x\n", ntohl(iph->daddr), iface->ip);
		if(icmp->type == 8 && ntohl(iph->daddr) == iface->ip)
			icmp_send_packet(packet, len, ICMP_ECHOREPLY, ICMP_NET_UNREACH);
		else
			ip_forward_packet(ntohl(iph->daddr), packet, len);
	}
	// 否则，进行数据包转发
	else
		ip_forward_packet(ntohl(iph->daddr), packet, len);

	// printf("\n\n");
}
