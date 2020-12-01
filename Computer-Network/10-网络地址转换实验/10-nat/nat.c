#include "nat.h"
#include "ip.h"
#include "icmp.h"
#include "tcp.h"
#include "rtable.h"
#include "log.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

static struct nat_table nat;

u32 ip_str2uint(const char *str_ip)
{
    unsigned int re = 0;
    unsigned char tmp = 0;
    while (1) {
        if (*str_ip != '\0' && *str_ip != '.') 
            tmp = tmp * 10 + *str_ip - '0';
        else 
        {
            re = (re << 8) + tmp;
            if (*str_ip == '\0')
                break;
            tmp = 0;
        }
        str_ip++;
    }
    return re;
}

u16 distribute_external_port() 
{

	for(u16 i = NAT_PORT_MIN; i <= NAT_PORT_MAX; i++)
	{
		if(nat.assigned_ports[i] == 0) {
			pthread_mutex_lock(&nat.lock);
			nat.assigned_ports[i] = 1;
			pthread_mutex_unlock(&nat.lock);		
			return i;
		}
	}

	return 0;
}

// get the interface from iface name
static iface_info_t *if_name_to_iface(const char *if_name)
{
	iface_info_t *iface = NULL;
	list_for_each_entry(iface, &instance->iface_list, list) {
		if (strcmp(iface->name, if_name) == 0)
			return iface;
	}

	log(ERROR, "Could not find the desired interface according to if_name '%s'", if_name);
	return NULL;
}

// determine the direction of the packet, DIR_IN / DIR_OUT / DIR_INVALID
static int get_packet_direction(char *packet)
{
	// fprintf(stdout, "TODO: determine the direction of this packet.\n");
	struct iphdr *iph = packet_to_ip_hdr(packet);

	u32 src_ip = ntohl(iph->saddr);
	u32 dst_ip = ntohl(iph->daddr);

	rt_entry_t *src_entry = longest_prefix_match(src_ip);
	rt_entry_t *dst_entry = longest_prefix_match(dst_ip);

	if(src_entry && dst_entry && src_entry->iface->ip == nat.internal_iface->ip && dst_entry->iface->ip == nat.external_iface->ip)
		return DIR_OUT;
	
	if(src_entry && src_entry->iface->ip == nat.external_iface->ip && dst_ip == nat.external_iface->ip)
		return DIR_IN;
	// if(src_entry)
	// {
	// 	printf("**** src_entry != NULL\n");
	// 	if(src_entry->iface->ip == nat.external_iface->ip)
	// 	{
	// 		printf("**** src_entry->iface->ip == nat.external_iface->ip\n");
	// 		if(dst_ip == nat.external_iface->ip)
	// 		{
	// 			printf("**** dst_ip == nat.external_iface->ip\n");
	// 			return DIR_IN;
	// 		}
	// 	}
	// }

	return DIR_INVALID;
}

// do translation for the in packet
struct nat_mapping *
do_translation_in(struct iphdr *iph, struct tcphdr *tcph, u32 rmt_ip, u16 rmt_port,
				 u32 dst_ip, u16 dst_port, u8 index)
{
	printf("**** Do translation in packet\n");
	printf("<<<< Dst_IP: "IP_FMT", Dst_Port: %d\n", HOST_IP_FMT_STR(dst_ip), dst_port);

	u8 flag = 0;
	struct nat_mapping *entry = NULL;
	list_for_each_entry(entry, &nat.nat_mapping_list[index], list)
	{
		if(dst_ip == entry->external_ip && dst_port == entry->external_port)
		{
			flag = 1;
			break;
		}
	}

	if(flag)
	{
		iph->daddr = htonl(entry->internal_ip);
		tcph->dport = htons(entry->internal_port);
		iph->checksum = ip_checksum(iph);
		tcph->checksum = tcp_checksum(iph, tcph);

		printf("<<<< After Translate: \n");
		printf("<<<< Dst_IP: "IP_FMT", Dst_Port: %d\n", HOST_IP_FMT_STR(entry->internal_ip), entry->internal_port);

		pthread_mutex_lock(&nat.lock);
		
		// update mapping entry
		entry->update_time = time(NULL);
		printf("<<<< Entry update_time update to %d\n", (int)entry->update_time);

		if(tcph->flags == TCP_FIN)
			entry->conn.external_fin = TCP_FIN;
		if(ntohl(tcph->seq) > entry->conn.external_seq_end)
			entry->conn.external_seq_end = ntohl(tcph->seq);
		if(ntohl(tcph->ack) > entry->conn.external_ack)
			entry->conn.external_ack = ntohl(tcph->ack);
		
		pthread_mutex_unlock(&nat.lock);		
		
		return entry;
	}
	else
	{
		struct dnat_rule *entry = NULL;
		list_for_each_entry(entry, &nat.rules, list)
		{
			if(dst_ip == entry->external_ip && dst_port == entry->external_port)
			{
				flag = 1;
				break;
			}
		}

		if(flag)
		{
			struct nat_mapping *mapping = (struct nat_mapping *)malloc(sizeof(struct nat_mapping));

			mapping->internal_ip = entry->internal_ip;
			mapping->internal_port = entry->internal_port;

			mapping->remote_ip = rmt_ip;
			mapping->remote_port = rmt_port;
			
			mapping->external_ip = entry->external_ip;
			mapping->external_port = entry->external_port;
			
			mapping->conn.external_fin = 0;	
			mapping->conn.external_seq_end = 0;					
			mapping->conn.external_ack = 0;

			mapping->conn.internal_fin = tcph->flags == TCP_FIN ? TCP_FIN : 0;
			mapping->conn.internal_seq_end = ntohl(tcph->seq);
			mapping->conn.internal_ack = ntohl(tcph->ack);

			list_add_tail(&mapping->list, &nat.nat_mapping_list[index]);

			// translate
			iph->daddr = htonl(mapping->internal_ip);
			tcph->dport = htons(mapping->internal_port);
			iph->checksum = ip_checksum(iph);
			tcph->checksum = tcp_checksum(iph, tcph);

			printf("<<<< After Translate: \n");
			printf("<<<< Dst_IP: "IP_FMT", Dst_Port: %d\n", HOST_IP_FMT_STR(mapping->internal_ip), mapping->internal_port);

			pthread_mutex_lock(&nat.lock);	

			if(tcph->flags == TCP_FIN)
				mapping->conn.external_fin = TCP_FIN;
			if(ntohl(tcph->seq) > mapping->conn.external_seq_end)
				mapping->conn.external_seq_end = ntohl(tcph->seq);
			if(ntohl(tcph->ack) > mapping->conn.external_ack)
				mapping->conn.external_ack = ntohl(tcph->ack);

			mapping->update_time = time(NULL);
			printf("<<<< Entry update_time update to %d\n", (int)mapping->update_time);

			pthread_mutex_unlock(&nat.lock);		
			
			return mapping;
		}

		return NULL;
	}
}

struct nat_mapping *
add_new_mapping(struct iphdr *iph, struct tcphdr *tcph,
					u32 rmt_ip, u16 rmt_port, u8 index)
{
	printf("**** Do add new mapping entry\n");

	struct nat_mapping *mapping = (struct nat_mapping *)malloc(sizeof(struct nat_mapping));
	u16 external_port = distribute_external_port();
	
	if(external_port)
		mapping->external_port = external_port;
	else {
		printf(">>>> No Avilable External Port\n");
		return NULL;
	}

	mapping->internal_ip = ntohl(iph->saddr);
	mapping->internal_port = ntohs(tcph->sport);

	mapping->remote_ip = rmt_ip;
	mapping->remote_port = rmt_port;
	
	mapping->external_ip = nat.external_iface->ip;
	
	mapping->update_time = time(NULL);
	printf(">>>> Entry update_time update to %d\n", (int)mapping->update_time);
	
	mapping->conn.external_fin = 0;	
	mapping->conn.external_seq_end = 0;					
	mapping->conn.external_ack = 0;
	
	mapping->conn.internal_fin = tcph->flags == TCP_FIN ? TCP_FIN : 0;
	mapping->conn.internal_seq_end = ntohl(tcph->seq);
	mapping->conn.internal_ack = ntohl(tcph->ack);
				
	list_add_tail(&mapping->list, &nat.nat_mapping_list[index]);

	printf(">>>> Internal_IP: "IP_FMT", Internal_Port: %d\n", HOST_IP_FMT_STR(mapping->internal_ip), mapping->internal_port);
	printf(">>>> External_IP: "IP_FMT", External_Port: %d\n", HOST_IP_FMT_STR(mapping->external_ip), mapping->external_port);

	return mapping;	
}

// do translation for the out packet
struct nat_mapping *
do_translation_out(struct iphdr *iph, struct tcphdr *tcph, u32 src_ip,
						 u16 src_port, u32 rmt_ip, u16 rmt_port, u8 index)
{
	printf("**** Do translation out packet\n");
	printf(">>>> Src_IP: "IP_FMT", Src_Port: %d\n", HOST_IP_FMT_STR(src_ip), src_port);
	printf(">>>> Rmt_IP: "IP_FMT", Rmt_Port: %d\n", HOST_IP_FMT_STR(rmt_ip), rmt_port);

	u8 flag = 0;
	struct nat_mapping *entry = NULL;
	list_for_each_entry(entry, &nat.nat_mapping_list[index], list)
	{
		if(src_ip == entry->internal_ip && src_port == entry->internal_port)
		{
			flag = 1;
			break;
		}
	}

	printf(">>>> Flag == %d\n", flag);
	if(flag) {
		iph->saddr = htonl(entry->external_ip);
		tcph->sport = htons(entry->external_port);
		iph->checksum = ip_checksum(iph);
		tcph->checksum = tcp_checksum(iph, tcph);
		
		printf(">>>> After Translate: \n");
		printf(">>>> Src_IP: "IP_FMT", Src_Port: %d\n", HOST_IP_FMT_STR(entry->external_ip), entry->external_port);

		// update mapping entry
		entry->update_time = time(NULL);
		printf(">>>> Entry update_time update to %d\n", (int)entry->update_time);

		if(tcph->flags == TCP_FIN)
			entry->conn.internal_fin = TCP_FIN;
		if(ntohl(tcph->seq) > entry->conn.internal_seq_end)
			entry->conn.internal_seq_end = ntohl(tcph->seq);
		if(ntohl(tcph->ack) > entry->conn.internal_ack)
			entry->conn.internal_ack = ntohl(tcph->ack);

		return entry;				
	}
	else
	{
		// build a new mapping
		struct nat_mapping *mapping = add_new_mapping(iph, tcph, rmt_ip, rmt_port, index);

		iph->saddr = htonl(mapping->external_ip);
		tcph->sport = htons(mapping->external_port);
		iph->checksum = ip_checksum(iph);
		tcph->checksum = tcp_checksum(iph, tcph);

		printf(">>>> After Translate: \n");
		printf(">>>> Src_IP: "IP_FMT", Src_Port: %d\n", HOST_IP_FMT_STR(mapping->external_ip), mapping->external_port);

		return mapping;
	}

	return NULL;
}

// do translation for the packet: replace the ip/port, recalculate ip & tcp
// checksum, update the statistics of the tcp connection
void do_translation(iface_info_t *iface, char *packet, int len, int dir)
{
	printf("**** Do translation packet\n");
	// fprintf(stdout, "TODO: do translation for this packet.\n");
	struct iphdr *iph = packet_to_ip_hdr(packet);
	struct tcphdr *tcph = (struct tcphdr *)((char *)iph + IP_HDR_SIZE(iph));

	u32 rmt_ip = dir == DIR_IN ? ntohl(iph->saddr) : ntohl(iph->daddr);
	u16 rmt_port = dir == DIR_IN ? ntohs(tcph->sport) : ntohs(tcph->dport);

	u32 dst_ip = ntohl(iph->daddr);
	u16 dst_port = ntohs(tcph->dport);

	u32 src_ip = ntohl(iph->saddr);
	u16 src_port = ntohs(tcph->sport);

	char buf[6];
	memcpy((char *)buf, (char *)&rmt_ip, 4);
	memcpy((char *)(buf + 4), (char *)&rmt_port, 2);

	u8 index = hash8(buf, 6);
	u8 flag = 0;

	struct nat_mapping *entry = NULL;
	if(dir == DIR_IN) 
		entry = do_translation_in(iph, tcph, rmt_ip, rmt_port, dst_ip, dst_port, index);	
	else
		entry = do_translation_out(iph, tcph, src_ip, src_port, rmt_ip, rmt_port, index);

	if(entry)
	{
		ip_send_packet(packet, len);
		printf("**** Send Packet Succeed\n");

		if(tcph->flags == TCP_RST && entry) 
		{
			pthread_mutex_lock(&nat.lock);
			nat.assigned_ports[entry->external_port] = 0;
			list_delete_entry(&(entry->list));
			free(entry);
			pthread_mutex_unlock(&nat.lock);
		}
	}
	else 
		printf("**** No Match NAT Entry\n");

	printf("\n");
}

void nat_translate_packet(iface_info_t *iface, char *packet, int len)
{
	int dir = get_packet_direction(packet);
	if (dir == DIR_INVALID) {
		log(ERROR, "invalid packet direction, drop it.");
		icmp_send_packet(packet, len, ICMP_DEST_UNREACH, ICMP_HOST_UNREACH);
		free(packet);
		return ;
	}

	struct iphdr *ip = packet_to_ip_hdr(packet);
	if (ip->protocol != IPPROTO_TCP) {
		log(ERROR, "received non-TCP packet (0x%0hhx), drop it", ip->protocol);
		free(packet);
		return ;
	}

	do_translation(iface, packet, len, dir);
}

// check whether the flow is finished according to FIN bit and sequence number
// XXX: seq_end is calculated by `tcp_seq_end` in tcp.h
static int is_flow_finished(struct nat_connection *conn)
{
    return (conn->internal_fin && conn->external_fin) && \
            (conn->internal_ack >= conn->external_seq_end) && \
            (conn->external_ack >= conn->internal_seq_end);
}


void do_nat_timeout()
{
	printf("++++ Now in do_nat_timeout\n");
	struct nat_mapping *entry = NULL, *q = NULL;
	for(u16 i = 0; i < HASH_8BITS; i++) 
	{
		pthread_mutex_lock(&nat.lock);
		list_for_each_entry_safe(entry, q, &(nat.nat_mapping_list[i]), list) 
		{
			u32 time_now = time(NULL);
			u32 timeout = time_now - entry->update_time;
			if(is_flow_finished(&entry->conn))
			{
				printf("++++ FIN Time Out One Entry \n");
				nat.assigned_ports[entry->external_port] = 0;					
				list_delete_entry(&(entry->list));
				free(entry);
			}
			else if(timeout > 60)
			{
				printf("++++ Now time is %d\n", time_now);
				printf("++++ %ds Timeout One Entry\n", timeout);
				nat.assigned_ports[entry->external_port] = 0;					
				list_delete_entry(&(entry->list));
				free(entry);
			}		
		}
		pthread_mutex_unlock(&nat.lock);
	}
}

// nat timeout thread: find the finished flows, remove them and free port
// resource
void *nat_timeout()
{
	u32 count = 0;
	while (1) {
		// fprintf(stdout, "TODO: sweep finished flows periodically.\n");

		printf("\n++++ Do NAT Time Out %d times\n", ++count);
		do_nat_timeout();

		sleep(1);
	}

	return NULL;
}

int get_iface_name(const char *strline, char *iface_name) 
{
	int i = 0, j;
	for( ; strline[i] != ' '; i++)
		;
	i++;

	for(j = 0; strline[i] != '\n'; i++, j++)
		iface_name[j] = strline[i];
	iface_name[j] = '\0';

	if(strline[0] == 'i')
		return 0;
	else if(strline[0] == 'e')
		return 1;
	else 
		return -1;
}

void rule_split(const char *strline, char *str_external_ip, char *str_external_port,
			 char *str_internal_ip, char *str_internal_port)
{
	int i = 0, j;
	for( ; strline[i] != ' '; i++)
		;
	i++;

	for(j = 0; strline[i] != ':'; i++, j++)
		str_external_ip[j] = strline[i];
	str_external_ip[j] = '\0';
	i++;

	for(j = 0; strline[i] != ' '; i++, j++)
		str_external_port[j] = strline[i];
	str_external_port[j] = '\0';
	i += 4;

	for(j = 0; strline[i] != ':'; i++, j++)
		str_internal_ip[j] = strline[i];
	str_internal_ip[j] = '\0';
	i++;

	for(j = 0; strline[i] != '\0'; i++, j++)
		str_internal_port[j] = strline[i];
	str_internal_port[j] = '\0';	

	// printf("external_ip: %s\n external_port: %s\n internal_ip: %s\n internal_port: %s\n",
	// 		 str_external_ip, str_external_port,
	// 		 str_internal_ip, str_internal_port
	// 	);	
}

void parse_add_new_rules(u32 external_ip, u16 external_port,
					u32 internal_ip, u16 internal_port)
{
	pthread_mutex_lock(&nat.lock);

	struct dnat_rule *rule = (struct dnat_rule *)malloc(sizeof(struct dnat_rule));

	rule->internal_ip = internal_ip ;
	rule->internal_port = internal_port;
	
	rule->external_ip = external_ip;
	rule->external_port = external_port;

	list_add_tail(&rule->list, &nat.rules);	
	
	pthread_mutex_unlock(&nat.lock);
}

int parse_config(const char *filename)
{
	// fprintf(stdout, "TODO: parse config file, including i-iface, e-iface (and dnat-rules if existing).\n");

	FILE *fp; 			
					
	if((fp = fopen(filename, "r")) == NULL) { 
		printf("Open Configure File Falied!\n"); 
		return -1; 
	}

	char strline[MAX_LINE];
	char iface_name[MAX_LINE];
	char str_external_ip[MAX_LINE];	
	char str_external_port[MAX_LINE];	
	char str_internal_ip[MAX_LINE];	
	char str_internal_port[MAX_LINE];

	while(!feof(fp))
	{
		fgets(strline, MAX_LINE, fp);
		// printf(">>>> strline: %s, strlen: %d\n", strline, (int)strlen(strline));
		// ignore invalid line
		if(strlen(strline) < 5)	
			break;
		
		u8 flag = get_iface_name(strline, iface_name);
		if(flag == 0) 
		{
			printf("**** Add an internal_iface, iface_name: %s\n", iface_name);
			nat.internal_iface = if_name_to_iface(iface_name);
		}
		else if(flag == 1)
		{
			printf("**** Add an external_iface, iface_name: %s\n", iface_name);	
			nat.external_iface = if_name_to_iface(iface_name);		
		}
		else 
			printf("**** Get iface_name Error\n");
		
		strline[0] = 0;
	}
	printf("\n");		

	while(!feof(fp)) {
		fgets(strline, MAX_LINE, fp);			
		// printf(">>>> strline: %s, strlen: %d\n", strline, (int)strlen(strline));
		// ignore invalid line
		if(strlen(strline) < 5)
			break;

		rule_split(strline, str_external_ip, str_external_port,
				str_internal_ip, str_internal_port);

		u32 external_ip = ip_str2uint(str_external_ip);
		u32 internal_ip = ip_str2uint(str_internal_ip);

		u16 external_port = (u16)atoi(str_external_port);
		u16 internal_port = (u16)atoi(str_internal_port);

		printf(">>>> Mapping: "IP_FMT":%d -> "IP_FMT":%d\n", HOST_IP_FMT_STR(external_ip), external_port,
													HOST_IP_FMT_STR(internal_ip), internal_port
				);

		parse_add_new_rules(external_ip, external_port, internal_ip, internal_port);
		
		strline[0] = 0;
	}

	fclose(fp);	
	
	return 0;
}

// initialize
void nat_init(const char *config_file)
{
	memset(&nat, 0, sizeof(nat));

	for (int i = 0; i < HASH_8BITS; i++)
		init_list_head(&nat.nat_mapping_list[i]);

	init_list_head(&nat.rules);

	// seems unnecessary
	memset(nat.assigned_ports, 0, sizeof(nat.assigned_ports));

	parse_config(config_file);

	pthread_mutex_init(&nat.lock, NULL);

	pthread_create(&nat.thread, NULL, nat_timeout, NULL);
}

void nat_exit()
{
	// fprintf(stdout, "TODO: release all resources allocated.\n");
	pthread_mutex_lock(&nat.lock);

	for (u8 i = 0; i < HASH_8BITS; i++) 
	{
		struct nat_mapping *entry, *q;
		list_for_each_entry_safe(entry, q, &nat.nat_mapping_list[i], list) 
		{
			list_delete_entry(&entry->list);
			free(entry);
		}
	}
	pthread_kill(nat.thread, SIGTERM);

	pthread_mutex_unlock(&nat.lock);
}
