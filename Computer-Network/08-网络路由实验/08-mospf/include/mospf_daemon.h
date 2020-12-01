#ifndef __MOSPF_DAEMON_H__
#define __MOSPF_DAEMON_H__

#include "base.h"
#include "types.h"
#include "list.h"

#define MAXINT 255

typedef struct {
	struct list_head list;
	u32 id;
	u32 prev_id;
    int index;
	iface_info_t* prev_iface;	
} V_Node;

typedef struct {
	u32 id;
	u32 port_ip; 
	u32 prev_id;
	int visited;
	iface_info_t* prev_iface;
} R_Node;

#define list_for_each_entry_prev(pos, head, member) \
    for (pos = list_entry((head)->prev, typeof(*pos), member);\
        	&pos->member != (head);\
        	pos = list_entry(pos->member.prev, typeof(*pos), member)) 

void mospf_init();
void mospf_run();
void handle_mospf_packet(iface_info_t *iface, char *packet, int len);


// 初始化拓扑
void Init_Graph(R_Node Graph[], int Distance[], struct list_head *Visited, int router_number);

// 添加访问到的节点
void add_visit_node(struct list_head *Visited, u32 id, u32 prev_id, 
				iface_info_t* prev_iface, int index);

// 访问到自己
void visit_self(R_Node Graph[], int Distance[], struct list_head *Visited, int router_number);

// 访问到其他节点
void visit_others(R_Node Graph[], int Distance[], struct list_head *Visited,
				int router_number, V_Node *current_visit_node);

// 简历新的路由表
void build_rtable(R_Node Graph[], struct list_head *Visited);

#endif
