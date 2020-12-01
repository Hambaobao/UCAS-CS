#include "packet.h"
#include "types.h"
#include "ether.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>

extern ustack_t *instance;

// send the packet out from iface
void iface_send_packet(iface_info_t *iface, char *packet, int len)
{
	struct sockaddr_ll addr;
	memset(&addr, 0, sizeof(struct sockaddr_ll));
	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = iface->index;
	addr.sll_halen = ETH_ALEN;
	addr.sll_protocol = htons(ETH_P_ARP);
	struct ether_header *eh = (struct ether_header *)packet;
	memcpy(addr.sll_addr, eh->ether_dhost, ETH_ALEN);

	if (sendto(iface->fd, packet, len, 0, (struct sockaddr *)&addr,
				sizeof(struct sockaddr_ll)) < 0) {
 		perror("Send raw packet failed");
	}
}

// broadcast the packet among all the interfaces except the one receiving the
// packet
void broadcast_packet(iface_info_t *iface, char *packet, int len)
{
	// TODO: implement the broadcast process here
	// fprintf(stdout, "TODO: implement the broadcast process here.\n");
	// printf("\n\nThe %d times call brodcast\n", count_brodcast++);

	// struct list_head list = instance->iface_list;
	struct list_head *list_ptr = &instance->iface_list;
	iface_info_t *entry;
	// printf("%d\n", iface->index);
	list_for_each_entry(entry, list_ptr, list) {
		// printf("index = %d\n", entry->index);
		if(entry->index != iface->index)
			iface_send_packet(entry, packet, len);
	}
}
