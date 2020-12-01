#include "arp.h"
#include "base.h"
#include "types.h"
#include "packet.h"
#include "ether.h"
#include "arpcache.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "log.h"

// send an arp request: encapsulate an arp request packet, send it out through
// iface_send_packet
void arp_send_request(iface_info_t *iface, u32 dst_ip)
{
	// fprintf(stderr, "TODO: send arp request when lookup failed in arpcache.\n");
	
	char *packet = (char *)malloc(sizeof(struct ether_arp) + sizeof(struct ether_header) + 18);
	struct ether_header *eth_h = (struct ether_header *)(packet);
	struct ether_arp *eth_arp = (struct ether_arp *)(packet + ETHER_HDR_SIZE);
	memcpy(eth_h->ether_shost, iface->mac, ETH_ALEN);

	for (int i = 0; i < ETH_ALEN; i++)
		eth_h->ether_dhost[i] = 0xff;

	eth_arp->arp_hln = 6;
	eth_arp->arp_pln = 4;
	eth_arp->arp_hrd = htons(ARPHRD_ETHER);
	eth_arp->arp_pro = htons(ETH_P_IP);
	eth_h->ether_type = htons(ETH_P_ARP);
	eth_arp->arp_op = htons(ARPOP_REQUEST);
	eth_arp->arp_spa = htonl(iface->ip);
	memcpy(eth_arp->arp_sha, iface->mac, ETH_ALEN);
	eth_arp->arp_tpa = htonl(dst_ip);

	iface_send_packet(iface, packet, sizeof(struct ether_arp) + sizeof(struct ether_header) + 18);

	// printf(">>>> Send ARP Request Succeed. SRC_IP==0x%x, DST_IP==0x%x\n", iface->ip, dst_ip);
}

// send an arp reply packet: encapsulate an arp reply packet, send it out
// through iface_send_packet
void arp_send_reply(iface_info_t *iface, struct ether_arp *req_hdr)
{
	// fprintf(stderr, "TODO: send arp reply when receiving arp request.\n");
	char *packet = malloc(sizeof(struct ether_header) + sizeof(struct ether_arp) + 18);

	// ***** Ether Begin *****
	struct ether_header *eth = (struct ether_header *)packet;
	memcpy(eth->ether_dhost, req_hdr->arp_sha, ETH_ALEN);
	memcpy(eth->ether_shost, iface->mac, ETH_ALEN);
	eth->ether_type = htons(ETH_P_ARP);
	// ***** Ether End *****

	// ***** ARP Header Begin *****
	struct ether_arp *eth_arp = (struct ether_arp *)(packet + ETHER_HDR_SIZE);
	memcpy(eth_arp , req_hdr, sizeof(struct ether_arp));
	memcpy(eth_arp->arp_sha, iface->mac, ETH_ALEN);	
	eth_arp->arp_spa = htonl(iface->ip);
	memcpy(eth_arp->arp_tha, req_hdr->arp_sha, ETH_ALEN);
	eth_arp->arp_tpa = req_hdr->arp_spa;
	eth_arp->arp_op = htons(ARPOP_REPLY);
	// ***** ARP Header End *****

	// fprintf(stderr, "send arp reply when receiving arp request.\n");
	iface_send_packet(iface, packet, sizeof(struct ether_header) + sizeof(struct ether_arp) + 18);

	// printf(">>>> Send ARP Reply Succeed. SRC_IP==0x%x, DST_IP==0x%x\n", iface->ip, ntohl(req_hdr->arp_spa));	
}

void handle_arp_packet(iface_info_t *iface, char *packet, int len)
{
	// fprintf(stderr, "TODO: process arp packet: arp request & arp reply.\n");
	struct ether_arp *eth_arp = (struct ether_arp *)(packet + ETHER_HDR_SIZE);
	
	// printf(">>>> Receive ARP Packet. DST_IP==0x%x, MY_IP==0x%x\n", ntohl(eth_arp->arp_tpa), iface->ip);
	switch(ntohs(eth_arp->arp_op)) {
		case ARPOP_REQUEST:
			if(ntohl(eth_arp->arp_tpa) == iface->ip) {
				// printf(">>>> Handle ARP_Request.\n");
				arpcache_insert(ntohl(eth_arp->arp_spa), eth_arp->arp_sha);
				arp_send_reply(iface, eth_arp);
				// printf(">>>> Done ARP Request and Replied.\n");
			}
			break;
		case ARPOP_REPLY:
			// printf(">>>> Handle ARP Reply.\n");
			arpcache_insert(ntohl(eth_arp->arp_spa), eth_arp->arp_sha);
			break;
		default:
			printf("Unkonwn ARP Packet, ignore!\n");
			break;			
	}

	// printf("\n\n");
}

// send (IP) packet through arpcache lookup 
//
// Lookup the mac address of dst_ip in arpcache. If it is found, fill the
// ethernet header and emit the packet by iface_send_packet, otherwise, pending 
// this packet into arpcache, and send arp request.
void iface_send_packet_by_arp(iface_info_t *iface, u32 dst_ip, char *packet, int len)
{
	// fprintf(stderr, "here is iface_send_packet_by_arp\n");
	struct ether_header *eh = (struct ether_header *)packet;
	memcpy(eh->ether_shost, iface->mac, ETH_ALEN);
	eh->ether_type = htons(ETH_P_IP);


	// printf(">>>> Sending Packet By ARP, DST_IP==0x%x\n", dst_ip);
	u8 dst_mac[ETH_ALEN];
	int found = arpcache_lookup(dst_ip, dst_mac);
	if (found) {
		// log(DEBUG, "found the mac of %x, send this packet", dst_ip);

		// printf(">>>> Found the mac of 0x%x, send this packet.\n", dst_ip);
		memcpy(eh->ether_dhost, dst_mac, ETH_ALEN);
		iface_send_packet(iface, packet, len);
	}
	else {
		// log(DEBUG, "lookup %x failed, pend this packet", dst_ip);
		// printf(">>>> Find the mac of 0x%x failed, pend this packet.\n", dst_ip);
		arpcache_append_packet(iface, dst_ip, packet, len);
	}
}
