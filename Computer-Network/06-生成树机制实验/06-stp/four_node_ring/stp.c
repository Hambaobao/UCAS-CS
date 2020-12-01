#include "stp.h"

#include "base.h"
#include "ether.h"
#include "utils.h"
#include "types.h"
#include "packet.h"
#include "log.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include <pthread.h>
#include <signal.h>

stp_t *stp;

const u8 eth_stp_addr[] = { 0x01, 0x80, 0xC2, 0x00, 0x00, 0x01 };

static bool stp_is_root_switch(stp_t *stp)
{
	return stp->designated_root == stp->switch_id;
}

static bool stp_port_is_designated(stp_port_t *p)
{
	return p->designated_switch == p->stp->switch_id &&
		p->designated_port == p->port_id;
}

static bool stp_port_is_root(stp_t *stp, stp_port_t *p)
{
	return stp->root_port && p->port_id == stp->root_port->port_id;
}

static const char *stp_port_state(stp_port_t *p)
{
	if (p->stp->root_port && \
			p->port_id == p->stp->root_port->port_id)
		return "ROOT";
	else if (p->designated_switch == p->stp->switch_id &&
		p->designated_port == p->port_id)
		return "DESIGNATED";
	else
		return "ALTERNATE";
}

static void stp_port_send_packet(stp_port_t *p, void *stp_msg, int msg_len)
{
	int pkt_len = ETHER_HDR_SIZE + LLC_HDR_SIZE + msg_len;
	char *pkt = malloc(pkt_len);

	// ethernet header
	struct ether_header *eth = (struct ether_header *)pkt;
	memcpy(eth->ether_dhost, eth_stp_addr, 6);
	memcpy(eth->ether_shost, p->iface->mac, 6);
	eth->ether_type = htons(pkt_len - ETHER_HDR_SIZE);

	// LLC header
	struct llc_header *llc = (struct llc_header *)(pkt + ETHER_HDR_SIZE);
	llc->llc_dsap = LLC_DSAP_SNAP;
	llc->llc_ssap = LLC_SSAP_SNAP;
	llc->llc_cntl = LLC_CNTL_SNAP;

	memcpy(pkt + ETHER_HDR_SIZE + LLC_HDR_SIZE, stp_msg, msg_len);

	iface_send_packet(p->iface, pkt, pkt_len);
}

static void stp_port_send_config(stp_port_t *p)
{
	stp_t *stp = p->stp;
	bool is_root = stp_is_root_switch(stp);
	if (!is_root && !stp->root_port) {
		return;
	}

	struct stp_config config;
	memset(&config, 0, sizeof(config));
	config.header.proto_id = htons(STP_PROTOCOL_ID);
	config.header.version = STP_PROTOCOL_VERSION;
	config.header.msg_type = STP_TYPE_CONFIG;
	config.flags = 0;
	config.root_id = htonll(stp->designated_root);
	config.root_path_cost = htonl(stp->root_path_cost);
	config.switch_id = htonll(stp->switch_id);
	config.port_id = htons(p->port_id);
	config.msg_age = htons(0);
	config.max_age = htons(STP_MAX_AGE);
	config.hello_time = htons(STP_HELLO_TIME);
	config.fwd_delay = htons(STP_FWD_DELAY);

	// log(DEBUG, "port %s send config packet.", p->port_name);
	stp_port_send_packet(p, &config, sizeof(config));
}

static void stp_send_config(stp_t *stp)
{
	for (int i = 0; i < stp->nports; i++) {
		stp_port_t *p = &stp->ports[i];
		if (stp_port_is_designated(p)) {
			stp_port_send_config(p);
		}
	}
}

static void stp_handle_hello_timeout(void *arg)
{
	// log(DEBUG, "hello timer expired, now = %llx.", time_tick_now());

	stp_t *stp = arg;
	stp_send_config(stp);
	stp_start_timer(&stp->hello_timer, time_tick_now());
}

static void stp_port_init(stp_port_t *p)
{
	stp_t *stp = p->stp;

	p->designated_root = stp->designated_root;
	p->designated_switch = stp->switch_id;
	p->designated_port = p->port_id;
	p->designated_cost = stp->root_path_cost;
}

void *stp_timer_routine(void *arg)
{
	while (true) {
		long long int now = time_tick_now();

		pthread_mutex_lock(&stp->lock);

		stp_timer_run_once(now);

		pthread_mutex_unlock(&stp->lock);

		usleep(100);
	}

	return NULL;
}

static int config_optimal_than_port(stp_port_t *p, struct stp_config *config)
{
	// config is optimal than this port
	return 	(p->designated_root > config->root_id) || 
			((p->designated_root == config->root_id) && (p->designated_cost > config->root_path_cost)) ||
			((p->designated_cost == config->root_path_cost) && (p->designated_switch > config->switch_id)) ||
			((p->designated_switch == config->switch_id) && (p->designated_port > config->port_id));
}

static int port1_optimal_than_port2(stp_port_t *p1, stp_port_t *p2)
{
	return 	(p2->designated_root > p1->designated_root) || 
			((p2->designated_root == p1->designated_root) && (p2->designated_cost > p1->designated_cost)) ||
			((p2->designated_cost == p1->designated_cost) && (p2->designated_switch > p1->designated_switch)) ||
			((p2->designated_switch == p1->designated_switch) && (p2->designated_port > p1->designated_port));	
}

static int nondesignated_optimal(stp_t *stp, stp_port_t *port)
{
	for (int i = 0; i < stp->nports; i++) 
	{
		stp_port_t *p = &stp->ports[i];
		if (!stp_port_is_designated(p) && port1_optimal_than_port2(p, port)) 
			return 0;
	}
	return 1;	
}

static void config_convert(struct stp_config *config)
{
	config->root_id = htonll(config->root_id);
	config->root_path_cost = htonl(config->root_path_cost);
	config->switch_id = htonll(config->switch_id);
	config->port_id = htons(config->port_id);
}

static void stp_handle_config_packet(stp_t *stp, stp_port_t *p,
		struct stp_config *config)
{
	// TODO: handle config packet here
	// fprintf(stdout, "TODO: handle config packet here.\n");
	
	// 判断初识状态是不是根节点
	bool start_is_root_switch = stp_is_root_switch(stp);

	// 将config中的网络字节序转换为本地字节序
	config_convert(config);

	// 1. 将config与p中的config信息进行优先级比较
	if(config_optimal_than_port(p, config)) {
	// 2. 如果收到的config优先级更高
		// i.	将本端口的config替换为收到的config消息, 将本端口改为非指定端口
		p->designated_switch = config->switch_id;
		p->designated_root = config->root_id;
		p->designated_port = config->port_id;
		p->designated_cost = config->root_path_cost;

		// ii.	更新stp的状态;
		int i;
		for (i = 0; i < stp->nports; i++) 
		{
			stp_port_t *p = &stp->ports[i];
			if (!stp_port_is_designated(p) && nondesignated_optimal(stp, p)) 
			{
				// 该端口为根端口
				// 选择通过root_port连接到根节点, 更新节点状态
				stp->root_port = p;
				stp->designated_root = p->designated_root;
				stp->root_path_cost = p->designated_cost + p->path_cost;

				break;
			}
		}

		// 如果不存在根端口，则该节点为根节点
		if(i == stp->nports) {
			stp->root_port = NULL;
			stp->designated_root = stp->switch_id;
			stp->root_path_cost = 0;
		}

		// iii.	更改stp中剩余端口的config
		for (i = 0; i < stp->nports; i++) 
		{
			stp_port_t *p = &stp->ports[i];
			if (!stp_port_is_root(stp, p) && !stp_port_is_designated(p) && nondesignated_optimal(stp, p)) 
			{
				// 该端口为指定端口
				p->designated_switch = stp->switch_id;
				p->designated_port = p->port_id;

				break;
			}
		}	

		for (i = 0; i < stp->nports; i++) 
		{
			stp_port_t *p = &stp->ports[i];
			if (stp_port_is_designated(p)) 
			{
				// 对所有指定端口，更新其认为的根节点和路经开销
				p->designated_root = stp->designated_root;
				p->designated_cost = stp->root_path_cost;
			}
		}

		// iv.	节点由根节点变为非根节点,停止hello计时器
		if(start_is_root_switch && !stp_is_root_switch(stp))
			stp_stop_timer(&stp->hello_timer);
		
		// v.	将更新后的config通过stp_send_config发送出去
		stp_send_config(stp);
	}
	else
	{
	// 3. 如果本端口config优先级更高
		// i. 该端口是指定端口，通过stp_port_send_config发送config消息
		stp_port_send_config(p);
	}
}

static void *stp_dump_state(void *arg)
{
#define get_switch_id(switch_id) (int)(switch_id & 0xFFFF)
#define get_port_id(port_id) (int)(port_id & 0xFF)

	pthread_mutex_lock(&stp->lock);

	bool is_root = stp_is_root_switch(stp);
	if (is_root) {
		log(INFO, "this switch is root."); 
	}
	else {
		log(INFO, "non-root switch, designated root: %04x, root path cost: %d.", \
				get_switch_id(stp->designated_root), stp->root_path_cost);
	}

	for (int i = 0; i < stp->nports; i++) {
		stp_port_t *p = &stp->ports[i];
		log(INFO, "port id: %02d, role: %s.", get_port_id(p->port_id), \
				stp_port_state(p));
		log(INFO, "\tdesignated ->root: %04x, ->switch: %04x, " \
				"->port: %02d, ->cost: %d.", \
				get_switch_id(p->designated_root), \
				get_switch_id(p->designated_switch), \
				get_port_id(p->designated_port), \
				p->designated_cost);
	}

	pthread_mutex_unlock(&stp->lock);

	exit(0);
}

static void stp_handle_signal(int signal)
{
	if (signal == SIGTERM) {
		log(DEBUG, "received SIGTERM, terminate this program.");
		
		pthread_t pid;
		pthread_create(&pid, NULL, stp_dump_state, NULL);
	}
}

void stp_init(struct list_head *iface_list)
{
	stp = malloc(sizeof(*stp));

	// set switch ID
	u64 mac_addr = 0;
	iface_info_t *iface = list_entry(iface_list->next, iface_info_t, list);
	for (int i = 0; i < sizeof(iface->mac); i++) {
		mac_addr <<= 8;
		mac_addr += iface->mac[i];
	}
	stp->switch_id = mac_addr | ((u64) STP_BRIDGE_PRIORITY << 48);

	stp->designated_root = stp->switch_id;
	stp->root_path_cost = 0;
	stp->root_port = NULL;

	stp_init_timer(&stp->hello_timer, STP_HELLO_TIME, \
			stp_handle_hello_timeout, (void *)stp);

	stp_start_timer(&stp->hello_timer, time_tick_now());

	stp->nports = 0;
	list_for_each_entry(iface, iface_list, list) {
		stp_port_t *p = &stp->ports[stp->nports];

		p->stp = stp;
		p->port_id = (STP_PORT_PRIORITY << 8) | (stp->nports + 1);
		p->port_name = strdup(iface->name);
		p->iface = iface;
		p->path_cost = 1;

		stp_port_init(p);

		// store stp port in iface for efficient access
		iface->port = p;

		stp->nports += 1;
	}

	pthread_mutex_init(&stp->lock, NULL);
	pthread_create(&stp->timer_thread, NULL, stp_timer_routine, NULL);

	signal(SIGTERM, stp_handle_signal);
}

void stp_destroy()
{
	pthread_kill(stp->timer_thread, SIGKILL);

	for (int i = 0; i < stp->nports; i++) {
		stp_port_t *port = &stp->ports[i];
		port->iface->port = NULL;
		free(port->port_name);
	}

	free(stp);
}

void stp_port_handle_packet(stp_port_t *p, char *packet, int pkt_len)
{
	stp_t *stp = p->stp;

	pthread_mutex_lock(&stp->lock);
	
	// protocol insanity check is omitted
	struct stp_header *header = (struct stp_header *)(packet + ETHER_HDR_SIZE + LLC_HDR_SIZE);

	if (header->msg_type == STP_TYPE_CONFIG) {
		stp_handle_config_packet(stp, p, (struct stp_config *)header);
	}
	else if (header->msg_type == STP_TYPE_TCN) {
		log(ERROR, "TCN packet is not supported in this lab.");
	}
	else {
		log(ERROR, "received invalid STP packet.");
	}

	pthread_mutex_unlock(&stp->lock);
}
