#include "mac.h"
#include "log.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

mac_port_map_t mac_port_map;

int equal_mac(u8 mac1[ETH_ALEN], u8 mac2[ETH_ALEN])
{
	for(int i = 0; i < ETH_ALEN; i++)
		if(mac1[i] != mac2[i])
			return 0;
	return 1;
}

// initialize mac_port table
void init_mac_port_table()
{
	bzero(&mac_port_map, sizeof(mac_port_map_t));

	for (int i = 0; i < HASH_8BITS; i++) {
		init_list_head(&mac_port_map.hash_table[i]);
	}

	pthread_mutex_init(&mac_port_map.lock, NULL);

	pthread_create(&mac_port_map.thread, NULL, sweeping_mac_port_thread, NULL);
}

// destroy mac_port table
void destory_mac_port_table()
{
	pthread_mutex_lock(&mac_port_map.lock);
	mac_port_entry_t *entry, *q;
	for (int i = 0; i < HASH_8BITS; i++) {
		list_for_each_entry_safe(entry, q, &mac_port_map.hash_table[i], list) {
			list_delete_entry(&entry->list);
			free(entry);
		}
	}
	pthread_mutex_unlock(&mac_port_map.lock);
}

// lookup the mac address in mac_port table
iface_info_t *lookup_port(u8 mac[ETH_ALEN])
{
	// TODO: implement the lookup process here
	// fprintf(stdout, "TODO: implement the lookup process here.\n");

	mac_port_entry_t *entry, *q;
	list_for_each_entry_safe(entry, q, &mac_port_map.hash_table[mac[ETH_ALEN - 1]], list) {
		if(equal_mac(entry->mac, mac)) {
			entry->visited = MAC_PORT_TIMEOUT;
			return entry->iface;
		}
	}
	return NULL;
}

// insert the mac -> iface mapping into mac_port table
void insert_mac_port(u8 mac[ETH_ALEN], iface_info_t *iface)
{
	// TODO: implement the insertion process here
	// fprintf(stdout, "TODO: implement the insertion process here.\n");



	mac_port_entry_t *entry, *q;
	for (int i = 0; i < HASH_8BITS; i++) {
		list_for_each_entry_safe(entry, q, &mac_port_map.hash_table[i], list) {
			if(equal_mac(entry->mac, mac)) {
				entry->visited = MAC_PORT_TIMEOUT;
				return ;
			}
		}
	}

	entry = safe_malloc(sizeof(mac_port_entry_t));
	memcpy(&entry->mac, mac, sizeof(entry->mac));
	entry->iface = iface;
	entry->visited = MAC_PORT_TIMEOUT;
	list_add_tail(&entry->list, &mac_port_map.hash_table[mac[ETH_ALEN - 1]]);
}

// dumping mac_port table
void dump_mac_port_table()
{
	mac_port_entry_t *entry = NULL;
	time_t now = time(NULL);

	fprintf(stdout, "dumping the mac_port table:\n");
	pthread_mutex_lock(&mac_port_map.lock);
	for (int i = 0; i < HASH_8BITS; i++) {
		list_for_each_entry(entry, &mac_port_map.hash_table[i], list) {
			fprintf(stdout, ETHER_STRING " -> %s, %d\n", ETHER_FMT(entry->mac), \
					entry->iface->name, (int)(now - entry->visited));
		}
	}

	pthread_mutex_unlock(&mac_port_map.lock);
}

// sweeping mac_port table, remove the entry which has not been visited in the
// last 30 seconds.
int sweep_aged_mac_port_entry()
{
	// TODO: implement the sweeping process here
	// fprintf(stdout, "TODO: implement the sweeping process here.\n");
	
	int rm_count = 0;
	mac_port_entry_t *entry, *q;
	pthread_mutex_lock(&mac_port_map.lock);
	for (int i = 0; i < HASH_8BITS; i++) {
		list_for_each_entry_safe(entry, q, &mac_port_map.hash_table[i], list) {
			if(entry->visited == 0) {
				list_delete_entry(&entry->list);
				free(entry->iface);
				free(entry);
				rm_count++;
				// printf(">>>>>> Free %d entry\n", rm_count);
			}
			else
				entry->visited -= 1;
		}
	}
	pthread_mutex_unlock(&mac_port_map.lock);
	return rm_count;
}

// sweeping mac_port table periodically, by calling sweep_aged_mac_port_entry
// uint32_t sweep_count = 0;
void *sweeping_mac_port_thread(void *nil)
{
	while (1) {
		// printf(">>>>>> The %d times run sweeping\n", ++sweep_count);
		sleep(1);
		int n = sweep_aged_mac_port_entry();
		// printf(">>>>>> The %d times run sweeping end\n", sweep_count);
		
		if (n > 0)
			log(DEBUG, "%d aged entries in mac_port table are removed.", n);
	}

	return NULL;
}
