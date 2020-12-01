#include "icmp.h"
#include "ip.h"
#include "rtable.h"
#include "arp.h"
#include "base.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// send icmp packet
void icmp_send_packet(const char *in_pkt, int len, u8 type, u8 code)
{
	/*fprintf(stderr, "TODO: malloc and send icmph packet.\n");*/
	struct ether_header *in_eh = (struct ether_header*)(in_pkt);
	struct iphdr *in_iph = packet_to_ip_hdr(in_pkt);
	int packet_len;

	// Reply Ping
	if (type == ICMP_ECHOREPLY && code == ICMP_NET_UNREACH){
		// printf(">>>> Receive Packet Ping Me.\n");
		packet_len = len;
	}
	else {
		packet_len = ETHER_HDR_SIZE+IP_BASE_HDR_SIZE+ICMP_HDR_SIZE+IP_HDR_SIZE(in_iph) + 8;
	}

	char *packet = (char *)malloc(packet_len);
	struct ether_header *eh = (struct ether_header *)(packet);
	struct iphdr *iph = packet_to_ip_hdr(packet);
	struct icmphdr *icmph = (struct icmphdr *)(packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE);

	memcpy(eh->ether_dhost, in_eh->ether_dhost, ETH_ALEN);
	memcpy(eh->ether_shost, in_eh->ether_dhost, ETH_ALEN);
	eh->ether_type = htons(ETH_P_IP);

	rt_entry_t *entry = longest_prefix_match(ntohl(in_iph->saddr));
	ip_init_hdr(iph, entry->iface->ip, ntohl(in_iph->saddr), packet_len-ETHER_HDR_SIZE, 1);

	icmph->type = type;
	icmph->code = code;
	
	char *rest_packet = (char *)(packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + ICMP_HDR_SIZE - 4);
	char *rest_in_pkt = (char *)(in_pkt + ETHER_HDR_SIZE + IP_HDR_SIZE(in_iph) + ICMP_HDR_SIZE - 4);
	
	if (type == ICMP_ECHOREPLY && code == ICMP_NET_UNREACH){
		int data_size = len - ETHER_HDR_SIZE - IP_HDR_SIZE(in_iph) - ICMP_HDR_SIZE + 4;
		memcpy(rest_packet, rest_in_pkt, data_size);
		icmph->checksum = icmp_checksum(icmph, data_size + ICMP_HDR_SIZE - 4);
	}
	else {
		int data_size = IP_HDR_SIZE(in_iph) + 8;
		memset(rest_packet, 0, 4);
		memcpy(rest_packet + 4, in_iph, data_size);
		icmph->checksum = icmp_checksum(icmph, data_size + ICMP_HDR_SIZE);
	}
	ip_send_packet(packet, packet_len);
}
