#include "mospf_daemon.h"
#include "mospf_proto.h"
#include "mospf_nbr.h"
#include "mospf_database.h"

#include "ip.h"
#include "ether.h"
#include "arp.h"
#include "rtable.h"
#include "packet.h"

#include "list.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

extern ustack_t *instance;

const u8 hello_dhost[ETH_ALEN] = {0x1, 0x0, 0x5e, 0x0, 0x0, 0x5}; // set to 01:00:5E:00:00:05
const u32 hello_daddr = 0xE0000005;	// set to 224.0.0.5 

pthread_mutex_t mospf_lock;

void mospf_init()
{
	pthread_mutex_init(&mospf_lock, NULL);

	instance->area_id = 0;
	// get the ip address of the first interface
	iface_info_t *iface = list_entry(instance->iface_list.next, iface_info_t, list);
	instance->router_id = iface->ip;
	instance->sequence_num = 0;
	instance->lsuint = MOSPF_DEFAULT_LSUINT;

	iface = NULL;
	list_for_each_entry(iface, &instance->iface_list, list) {
		iface->helloint = MOSPF_DEFAULT_HELLOINT;
		init_list_head(&iface->nbr_list);
	}

	init_mospf_db();
}

void show_database()
{
	fprintf(stdout, "RID\t\tNetwork\t\tMask\t\tNeighbor\n");
	fprintf(stdout, "--------------------------------------------------------\n");
	mospf_db_entry_t *db_entry;
	list_for_each_entry(db_entry, &mospf_db, list){
		for(int i = 0; i < db_entry->nadv; i++)
		  fprintf(stdout, IP_FMT"\t"IP_FMT"\t"IP_FMT"\t"IP_FMT"\n",
					  HOST_IP_FMT_STR(db_entry->rid),
					  HOST_IP_FMT_STR(db_entry->array[i].network), 
					  HOST_IP_FMT_STR(db_entry->array[i].mask),
					  HOST_IP_FMT_STR(db_entry->array[i].rid)
				 );
	}
	fprintf(stdout, "\n\n");
}

void send_mospf_lsu() 
{
	pthread_mutex_unlock(&mospf_lock);
	
	u32 nadv = 0;
	iface_info_t *iface;
	list_for_each_entry(iface, &instance->iface_list, list) {
		mospf_nbr_t *entry = NULL;
		int flag = 0;

		list_for_each_entry(entry, &iface->nbr_list,list) {
			nadv++;
			flag = 1;
		}
		// 该端口无邻居,即链接host
		if(flag == 0) 
			nadv++;
	}		
	
	int index = 0;
	struct mospf_lsa *array_lsa = (struct mospf_lsa*)malloc(nadv * MOSPF_LSA_SIZE);
	list_for_each_entry(iface, &instance->iface_list, list) {
		int flag = 0;
		mospf_nbr_t *entry;
		list_for_each_entry(entry, &iface->nbr_list,list) {
			array_lsa[index].network = htonl(entry->nbr_mask & entry->nbr_ip);
			array_lsa[index].mask = htonl(entry->nbr_mask);
			array_lsa[index].rid = htonl(entry->nbr_id);
			index++;
			flag = 1;
		}
		// HOST
		if(flag == 0) { 
			array_lsa[index].network = htonl(iface->ip & iface->mask);
			array_lsa[index].mask = htonl(iface->mask);				
			array_lsa[index].rid = htonl(0);
			index++;
		}
	}	

	pthread_mutex_unlock(&mospf_lock);
	
	list_for_each_entry(iface, &instance->iface_list, list){
		mospf_nbr_t *entry;
		list_for_each_entry(entry, &iface->nbr_list,list){
			
			int len = ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + MOSPF_HDR_SIZE + MOSPF_LSU_SIZE + nadv * MOSPF_LSA_SIZE;
			char *packet = (char *)malloc(len);				
			memset(packet, 0, len);

			struct ether_header* eth = (struct ether_header*)(packet);
			memcpy(eth->ether_shost, iface->mac, ETH_ALEN);
			eth->ether_type = htons(ETH_P_IP);	
			
			struct iphdr *iph = (struct iphdr *)(packet + ETHER_HDR_SIZE);	
			ip_init_hdr(iph, iface->ip, entry->nbr_ip, len - ETHER_HDR_SIZE, IPPROTO_MOSPF);

			struct mospf_hdr *mospf = (struct mospf_hdr *)((char *)iph + IP_HDR_SIZE(iph));									
			mospf_init_hdr(mospf, MOSPF_TYPE_LSU, MOSPF_HDR_SIZE, instance->router_id, instance->area_id);		
			
			struct mospf_lsu *lsu = (struct mospf_lsu *)((char *)mospf + MOSPF_HDR_SIZE);
			mospf_init_lsu(lsu, nadv);
			
			struct mospf_lsa *lsa = (struct mospf_lsa *)((char*)lsu + MOSPF_LSU_SIZE);
			if(nadv)
				memcpy(lsa, (char *)array_lsa, nadv * MOSPF_LSA_SIZE);		

			mospf->checksum = mospf_checksum(mospf);
			
			ip_send_packet(packet, len);		
		}		
	}
	
	instance->sequence_num++;

	if(nadv) 
		free(array_lsa);
}


void *sending_mospf_hello_thread(void *param);
void *sending_mospf_lsu_thread(void *param);
void *checking_nbr_thread(void *param);
void *checking_database_thread(void *param);
void *generate_rtable_thread(void *param);

void mospf_run()
{
	pthread_t hello, lsu, nbr, db, rtb;
	pthread_create(&hello, NULL, sending_mospf_hello_thread, NULL);
	pthread_create(&lsu, NULL, sending_mospf_lsu_thread, NULL);
	pthread_create(&nbr, NULL, checking_nbr_thread, NULL);
	pthread_create(&db, NULL, checking_database_thread, NULL);
	pthread_create(&rtb, NULL, generate_rtable_thread, NULL);
}

void add_visit_node(struct list_head *Visited, u32 id, u32 prev_id, 
				iface_info_t* prev_iface, int index)
{
	V_Node *temp_node = (V_Node *)malloc(sizeof(V_Node));
	temp_node->id = id;
	temp_node->prev_id = prev_id;
	temp_node->prev_iface = prev_iface;
	temp_node->index = index;
	list_add_head(&temp_node->list, Visited);		
}

void Init_Graph(R_Node Graph[], int Distance[], struct list_head *Visited, int router_number)
{
	// 初始化Graph
	int index = 0;
	mospf_db_entry_t *entry;
	list_for_each_entry(entry, &mospf_db, list) {
		Graph[index].id = entry->rid;
		Graph[index].prev_id = 0;
		Graph[index].visited = 0;
		Graph[index].port_ip = 0;
		Graph[index].prev_iface = NULL;
		index++;
	}

	// 初始化Distance
	for(int i = 0; i < router_number; i++) 
		Distance[i] = -1;

	// 初始化Visited
	init_list_head(Visited);
}

void visit_self(R_Node Graph[], int Distance[], struct list_head *Visited, int router_number)
{
	iface_info_t *inter_face;
	// 遍历本节点所有接口
	list_for_each_entry(inter_face, &instance->iface_list, list) {
		mospf_nbr_t *entry_nbr;	
		// 遍历每个接口的邻居节点				
		list_for_each_entry(entry_nbr, &inter_face->nbr_list,list) {
			int index;
			// 在已知的所有路由器节点中查找
			for(index = 0; index < router_number; index++) {
				// 如果在图中找到给该接口发送消息的路由器节点
				// 记下在Graph中的位置index
				if(Graph[index].id == entry_nbr->nbr_id){
					break;
				}
			}

			// 修改本节点到发送消息的节点的Distance和Graph信息
			// 相邻节点距离为1
			Distance[index] = 1;
			// 发送消息的节点的port_ip改为收到的消息的源ip地址，即发送消息节点端口的ip
			Graph[index].port_ip = entry_nbr->nbr_ip;
			// 发送消息的节点的prev节点改为本节点
			Graph[index].prev_id = instance->router_id;
			// 发送消息的节点的prev_iface改为本端口，即通过本端口收发信息
			Graph[index].prev_iface = inter_face;
		}	
	}

	// 选择一个邻居节点 
	int lucky;
	for(lucky = 0; lucky < router_number; lucky++) {
		if(Distance[lucky] == 1) 
			break;
	}

	// Visit这个节点，并插入Visted链表			
	add_visit_node(Visited, Graph[lucky].id, instance->router_id, Graph[lucky].prev_iface, lucky);
	Graph[lucky].visited = 1;	
}

void visit_others(R_Node Graph[], int Distance[], struct list_head *Visited, int router_number, V_Node *current_visit_node)
{
	mospf_db_entry_t *entry_db;
	// 遍历链路状态数据库，在数据库中找到现在访问的这个节点发来的消息
	list_for_each_entry(entry_db, &mospf_db, list) {
		if(entry_db->rid == current_visit_node->id) {
			break;
		} 
	}

	int lsa_index;
	// 遍历该数据下的所有LSA
	for(lsa_index = 0; lsa_index < entry_db->nadv; lsa_index++) {
		int current_visit_node_index = -1;
		int unvisited_index = -1;
		int flag = 0;

		int index;
		for(index = 0; index < router_number; index++) {
			// 找到现在正在访问的节点，把这个节点在图中的位置记录在current_visit_node_index中
			if(Graph[index].id == current_visit_node->id) 
				current_visit_node_index = index;

			// 如果图中某个节点是现在正在访问的节点的邻居且还没有访问过，把这个节点的位置记录在unvisited_index中
			if((Graph[index].id == entry_db->array[lsa_index].rid) && Graph[index].visited == 0) 
			{
				flag = 1;
				unvisited_index = index;
			}
		}

		// 如果这次没有找到正在访问的节点的还没有访问的邻居，就跳过
		if(flag == 0) 
			continue;

		// 如果找到了正在访问的节点的还没有访问的邻居，
		// 且这个没有访问的邻居的距离还处于初始状态没有变过，
		// 或者已经有了距离但是经过现在正在访问的这个节点的路径更短，
		// 那么就修改相关信息
		if(Distance[unvisited_index] == -1 || Distance[current_visit_node->index] + 1 < Distance[unvisited_index]) 
		{
			Distance[unvisited_index] = Distance[current_visit_node->index] + 1;
			// 要到达这个unvisited节点，需要通过与current_visit节点相同的端口ip（邻居的邻居）
			Graph[unvisited_index].port_ip = Graph[current_visit_node_index].port_ip;
			Graph[unvisited_index].prev_id = current_visit_node->id;
			// 要到达这个unvisited节点，需要通过与current_visit节点相同的端口（邻居的邻居）
			Graph[unvisited_index].prev_iface = current_visit_node->prev_iface;
		}	

	}
	
	// 找一个到本节点距离最小最小且没有访问的节点插入				
	int min = MAXINT;
	int lucky;
	for(int i = 0; i < router_number; i++) {
		if(Graph[i].visited == 0) {
			if(Distance[i] > 0 && Distance[i] < min) 
			{
				min = Distance[i];
				lucky = i;
			}
		}
	}

	add_visit_node(Visited, Graph[lucky].id, Graph[lucky].prev_id, Graph[lucky].prev_iface, lucky);
	Graph[lucky].visited = 1;			
}

void build_rtable(R_Node Graph[], struct list_head *Visited)
{
	// 构造路由表
	iface_info_t *inter_face;
	// 加入本地网络
	list_for_each_entry(inter_face, &instance->iface_list, list) {
		mospf_nbr_t *entry_temp;
		int flag = 0;
		list_for_each_entry(entry_temp, &inter_face->nbr_list,list) {
			// 如果本端口有邻居节点，则加入到邻居节点的路由表项
			flag = 1;
			add_rt_entry(new_rt_entry(entry_temp->nbr_ip & entry_temp->nbr_mask, entry_temp->nbr_mask, 0, inter_face));
		}
		// 如果本端口没有邻居节点，则加入本端口对应的路由表项（连接到主机）
		if(flag == 0)	
			add_rt_entry(new_rt_entry(inter_face->ip & inter_face->mask, inter_face->mask, 0, inter_face));		
	}

	// 加入非本地网络
	V_Node *entry_nd;
	// 遍历已经遍历过的所有节点
	list_for_each_entry_prev(entry_nd, Visited, list) {
		mospf_db_entry_t *entry_db;
		// 从本地链路状态数据库中查找
		list_for_each_entry(entry_db, &mospf_db, list) {
			// 如果找到非本地网络对应的路由器节点，就跳出循环
			if(entry_db->rid == entry_nd->id)
				break;
		}

		for(int index = 0; index < entry_db->nadv; index++) {
			// 如果路由表中没有到达链路状态数据库指向的网络的条目，则加入新的路由表项
			if(longest_prefix_match(entry_db->array[index].network) == NULL) 
				add_rt_entry(new_rt_entry(entry_db->array[index].network, entry_db->array[index].mask, 
											Graph[entry_nd->index].port_ip, entry_nd->prev_iface));					
		}
	}	
}

void generate_rtable()
{
	pthread_mutex_lock(&mospf_lock);

	// 统计节点个数
	int router_number = 0;
	mospf_db_entry_t *entry;
	list_for_each_entry(entry, &mospf_db, list) {
		router_number++;
	}

	R_Node Graph[router_number];
	int Distance[router_number];
	struct list_head Visited;

	// 初始化拓扑
	Init_Graph(Graph, Distance, &Visited, router_number);

	// 插入本节点		
	add_visit_node(&Visited, instance->router_id, -1, NULL, 0);
	
	// 循环router_number次，即可添加全部节点
	for(int i = 0; i < router_number; i++) {
		// 第一次，Visited中只有本机一个节点，所以entry就是本节点
		// 之后每次循环，会得到其他节点						
		V_Node *current_visit_node = list_entry(Visited.next, V_Node, list);
				
		if(current_visit_node->id == instance->router_id)
			// 如果当前visit的节点是本节点 
			visit_self(Graph, Distance, &Visited, router_number);	
		else 
			// 如果当前visit的节点是其他节点
			visit_others(Graph, Distance, &Visited, router_number, current_visit_node);
	}

	// 清空原来的路由表
	clear_rtable();	
	// 构造新的路由表
	build_rtable(Graph, &Visited);

	pthread_mutex_unlock(&mospf_lock);
}


void *generate_rtable_thread(void *param)
{
	while(1) {
		sleep(10);

		// 重新生成路由表
		generate_rtable();

		// 为方便观察实验结果，打印路由表和打印链路状态数据库二选一，根据需要选择注释
		print_rtable();
		printf("\n\n");
	}
}

void *sending_mospf_hello_thread(void *param)
{
	// fprintf(stdout, "TODO: send mOSPF Hello message periodically.\n");

	while(1) {
		sleep(MOSPF_DEFAULT_HELLOINT);
		pthread_mutex_lock(&mospf_lock);
		
		iface_info_t *iface;
		list_for_each_entry(iface, &instance->iface_list, list){
			int packet_len = ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + MOSPF_HDR_SIZE + MOSPF_HELLO_SIZE;
			char *packet = (char *)malloc(packet_len);

			struct ether_header *ether = (struct ether_header *)packet;
			memcpy(ether->ether_shost, iface->mac, ETH_ALEN);
			memcpy(ether->ether_dhost, hello_dhost, ETH_ALEN);
			ether->ether_type = htons(ETH_P_IP);

			struct iphdr *ip = packet_to_ip_hdr(packet);
			ip_init_hdr(ip, iface->ip, hello_daddr, packet_len - ETHER_HDR_SIZE, IPPROTO_MOSPF);

			struct mospf_hdr *mospf = (struct mospf_hdr *)((char *)ip + IP_HDR_SIZE(ip));
			mospf_init_hdr(mospf, MOSPF_TYPE_HELLO, MOSPF_HDR_SIZE + MOSPF_HELLO_SIZE, instance->router_id, instance->area_id);

			struct mospf_hello *hello = (struct mospf_hello *)((char *)mospf + MOSPF_HDR_SIZE);
			mospf_init_hello(hello, iface->mask);

			mospf->checksum = mospf_checksum(mospf);
			iface_send_packet(iface, packet, packet_len);
		}
		pthread_mutex_unlock(&mospf_lock);
	}
	return NULL;
}

void *checking_nbr_thread(void *param)
{
	// fprintf(stdout, "TODO: neighbor list timeout operation.\n");

	while(1) {
		sleep(1);
		
		int changed = 0;
		iface_info_t *iface;
		pthread_mutex_lock(&mospf_lock);
		list_for_each_entry(iface, &instance->iface_list, list) {
			mospf_nbr_t *entry, *entry_q;
			list_for_each_entry_safe(entry, entry_q,&iface->nbr_list,list){
				if(entry->alive > 3 * MOSPF_DEFAULT_HELLOINT) {
					list_delete_entry(&entry->list);
					free(entry);
					iface->num_nbr--;
					changed = 1;
				}
				else
				entry->alive++;
			}
		}

		pthread_mutex_unlock(&mospf_lock);

		if(changed) 
			send_mospf_lsu();
	}

	return NULL;
}

void *checking_database_thread(void *param)
{
	// fprintf(stdout, "TODO: link state database timeout operation.\n");

	while(1) {
		sleep(1);
		pthread_mutex_lock(&mospf_lock);

		if (!list_empty(&mospf_db)) {
			mospf_db_entry_t *db_entry, *db_q;	
			list_for_each_entry_safe(db_entry, db_q, &mospf_db, list) {
				if (time(NULL) - db_entry->alive > MOSPF_DATABASE_TIMEOUT) 
				{
					list_delete_entry(&db_entry->list);
					free(db_entry);
				}
			}
		}
		pthread_mutex_unlock(&mospf_lock);		

	}

	return NULL;
}

void handle_mospf_hello(iface_info_t *iface, const char *packet, int len)
{
	// fprintf(stdout, "TODO: handle mOSPF Hello message.\n");
	
	struct iphdr *ip = (struct iphdr *)(packet + ETHER_HDR_SIZE);
	struct mospf_hdr *mospf = (struct mospf_hdr *)((char *)ip + IP_HDR_SIZE(ip));
	struct mospf_hello *hello = (struct mospf_hello *)((char *)mospf + MOSPF_HDR_SIZE);

	pthread_mutex_lock(&mospf_lock);

	int flag = 0;
	mospf_nbr_t *entry;
	list_for_each_entry(entry, &iface->nbr_list, list) {
		if(entry->nbr_id == ntohl(mospf->rid)) {
			entry->alive = 0;
			flag = 1;
			break;
		}
	}

	pthread_mutex_unlock(&mospf_lock);

	if(flag == 0) {
		mospf_nbr_t *new = (mospf_nbr_t *)malloc(sizeof(mospf_nbr_t));

		new->alive = 0;
		new->nbr_id = ntohl(mospf->rid);
		new->nbr_ip = ntohl(ip->saddr);
		new->nbr_mask = ntohl(hello->mask);

		pthread_mutex_lock(&mospf_lock);		
		list_add_tail(&new->list, &iface->nbr_list);
		iface->num_nbr++;
		pthread_mutex_unlock(&mospf_lock);
		
		// 有新的邻居加入
		send_mospf_lsu();
	}
}

void *sending_mospf_lsu_thread(void *param)
{
	// fprintf(stdout, "TODO: send mOSPF LSU message periodically.\n");

	while(1) {
		sleep(MOSPF_DEFAULT_LSUINT);
		send_mospf_lsu();
	}

	return NULL;
}

void add_db_entry(u32 rid, u16 seq_num, int nadv, struct mospf_lsa *lsa)
{
	// 如果原来没有这台路由器发来的数据，则新添加数据项
	mospf_db_entry_t *new = (mospf_db_entry_t *)malloc(sizeof(mospf_db_entry_t));
	new->rid = rid;
	new->seq = seq_num;
	new->nadv = nadv;
	new->alive = time(NULL);
	new->array = (struct mospf_lsa *)malloc(nadv * MOSPF_LSA_SIZE);		
	for(int i = 0; i < new->nadv; i++){
		new->array[i].network = ntohl(lsa[i].network);
		new->array[i].mask = ntohl(lsa[i].mask);
		new->array[i].rid = ntohl(lsa[i].rid);
	}
	list_add_tail(&new->list, &mospf_db);	
}

void update_db_entry(mospf_db_entry_t *entry, u16 seq_num, int nadv, struct mospf_lsa *lsa)
{
	// 如果原来有这台路由器发来的数据，则更新数据项和到达时间
	entry->seq = seq_num;
	entry->nadv = nadv;
	entry->alive = time(NULL);
	free(entry->array);
	entry->array = (struct mospf_lsa *)malloc(nadv * MOSPF_LSA_SIZE);	
	for(int i = 0; i < entry->nadv; i++) {
		entry->array[i].network = ntohl(lsa[i].network);
		entry->array[i].mask = ntohl(lsa[i].mask);
		entry->array[i].rid = ntohl(lsa[i].rid);
	}	
}

void forward_lsu_packet(iface_info_t *iface, char *packet, int len)
{
	iface_info_t *inter_face;
	list_for_each_entry(inter_face, &instance->iface_list, list) {
		if(inter_face->index != iface->index) {
			mospf_nbr_t *entry;
			list_for_each_entry(entry, &inter_face->nbr_list, list) {
				char *new_packet = (char *)malloc(len);
				memcpy(new_packet, packet, len);

				struct ether_header *eth = (struct ether_header *)new_packet;    
				memcpy(eth->ether_shost, inter_face->mac, ETH_ALEN);

				struct iphdr *iph = (struct iphdr *)(new_packet + ETHER_HDR_SIZE);
				ip_init_hdr(iph, inter_face->ip, entry->nbr_ip, len - ETHER_HDR_SIZE, IPPROTO_MOSPF);
				
				// TTL改变了
				struct mospf_hdr *mospfh = (struct mospf_hdr *)((char *)iph + IP_HDR_SIZE(iph));
				mospfh->checksum = mospf_checksum(mospfh);					
				
				ip_send_packet(new_packet, len);
			}
		}
	}
}

void handle_mospf_lsu(iface_info_t *iface, char *packet, int len)
{
	// fprintf(stdout, "TODO: handle mOSPF LSU message.\n");

	struct iphdr *ip = (struct iphdr *)(packet + ETHER_HDR_SIZE);
	struct mospf_hdr *mospf = (struct mospf_hdr *)((char *)ip + IP_HDR_SIZE(ip));
	struct mospf_lsu *lsu = (struct mospf_lsu *)((char *)mospf + MOSPF_HDR_SIZE);
	struct mospf_lsa *lsa = (struct mospf_lsa *)((char *)lsu + MOSPF_LSU_SIZE);

	int mospf_rid = ntohl(mospf->rid);
	int seq_num = ntohs(lsu->seq);
    int nadv = ntohl(lsu->nadv);

	// 如果收到的mospf消息不是自己发出去的，则进行处理
	if(mospf_rid != instance->router_id) {
		pthread_mutex_lock(&mospf_lock);

		int flag = 0;
		mospf_db_entry_t *entry = NULL;
		list_for_each_entry(entry, &mospf_db, list) {
			if(entry->rid == mospf_rid)	{
				flag = 1;	
				break;
			}					
		}
		if(flag == 0) 
			// 如果原来没有这台路由器发来的数据，则新添加数据项
			add_db_entry(mospf_rid, seq_num, nadv, lsa);
		else if(seq_num > entry->seq)
			// 如果原来有这台路由器发来的数据，则更新数据项和到达时间
			update_db_entry(entry, seq_num, nadv, lsa);	

		pthread_mutex_unlock(&mospf_lock);
	}

	// 为方便观察实验结果，打印路由表和打印链路状态数据库二选一，根据需要选择注释
	// show_database();

	// 如果TTL不为0
    if(--lsu->ttl) 
		forward_lsu_packet(iface, packet, len);
}

void handle_mospf_packet(iface_info_t *iface, char *packet, int len)
{
	struct iphdr *ip = (struct iphdr *)(packet + ETHER_HDR_SIZE);
	struct mospf_hdr *mospf = (struct mospf_hdr *)((char *)ip + IP_HDR_SIZE(ip));

	if (mospf->version != MOSPF_VERSION) {
		log(ERROR, "received mospf packet with incorrect version (%d)", mospf->version);
		return ;
	}
	if (mospf->checksum != mospf_checksum(mospf)) {
		log(ERROR, "received mospf packet with incorrect checksum");
		return ;
	}
	if (ntohl(mospf->aid) != instance->area_id) {
		log(ERROR, "received mospf packet with incorrect area id");
		return ;
	}

	switch (mospf->type) {
		case MOSPF_TYPE_HELLO:
			handle_mospf_hello(iface, packet, len);
			break;
		case MOSPF_TYPE_LSU:
			handle_mospf_lsu(iface, packet, len);
			break;
		default:
			log(ERROR, "received mospf packet with unknown type (%d).", mospf->type);
			break;
	}
}
