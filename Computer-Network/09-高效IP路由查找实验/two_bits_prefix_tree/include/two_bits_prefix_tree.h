#include <stdio.h>
#include <stdlib.h>
#include "type.h"

#define MOSTSIGBIT 0x80000000
#define MOSTSIGBITS 0xc0000000

#define MOST00 0x0
#define MOST01 0x40000000
#define MOST10 0x80000000
#define MOST11 0xc0000000

#define IPNUMBER    697886

typedef struct Node {
    u32 ip;
    u16 prefix;             // 从0开始计算，第prefix位不相同
    u16 odd_port;
    u16 even_port;
    struct Node *child00;
    struct Node *child01;
    struct Node *child10;
    struct Node *child11;
} Node_t;

u32 IP_Database[IPNUMBER];
u16 Mask_Database[IPNUMBER];
u16 Port_Database[IPNUMBER];
u16 Port_Results[IPNUMBER];

u32 IP_count;
u32 Memory_Cost;

#define IP_FMT	"%hhu.%hhu.%hhu.%hhu"
#define LE_IP_FMT_STR(ip) ((u8 *)&(ip))[3], \
						  ((u8 *)&(ip))[2], \
 						  ((u8 *)&(ip))[1], \
					      ((u8 *)&(ip))[0]

#define BE_IP_FMT_STR(ip) ((u8 *)&(ip))[0], \
						  ((u8 *)&(ip))[1], \
 						  ((u8 *)&(ip))[2], \
					      ((u8 *)&(ip))[3]

#define NET_IP_FMT_STR(ip)	BE_IP_FMT_STR(ip)

#if __BYTE_ORDER == __LITTLE_ENDIAN
#	define HOST_IP_FMT_STR(ip)	LE_IP_FMT_STR(ip)
#elif __BYTE_ORDER == __BIG_ENDIAN
#	define HOST_IP_FMT_STR(ip)	BE_IP_FMT_STR(ip)
#endif


void init_node(Node_t *node, u32 ip, u16 prefix, u16 port);
void insert_node(Node_t *node);
int construct_tree();

Node_t *look_up_ip(u32 ip, u16 mask);
void look_up_one();
int look_up_all();
int check_results();
