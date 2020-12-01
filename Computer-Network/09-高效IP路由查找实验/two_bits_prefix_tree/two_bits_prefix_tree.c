#include "two_bits_prefix_tree.h"
#include "tool.h"
#include "type.h"
#include <sys/time.h>

Node_t *root;

void init_node(Node_t *node, u32 ip, u16 prefix, u16 port) 
{
    node->ip = ip;
    node->prefix = prefix;
    node->odd_port = prefix % 2 ? port : -1;
    node->even_port = prefix % 2 ? -1 : port;
    node->child00 = NULL;
    node->child01 = NULL;
    node->child10 = NULL;
    node->child11 = NULL;
}

int construct_tree()
{
	char strline[MAX_LINE];	
	FILE *fp; 			
					
	if((fp = fopen("forwarding-table.txt","r")) == NULL) { 
		printf("Open File Falied!\n"); 
		return -1; 
	}

	char str_ip[STRIPLEN];
	char str_mask[STRMASKLEN];
	char str_port[STRPORTLEN];
	u32 ip;
    u16 mask, port;

    Memory_Cost = 0;
	root = (Node_t *)malloc(sizeof(Node_t));
    Memory_Cost += sizeof(Node_t);
    init_node(root, 0, 0, 0);

    IP_count = 0;
    printf("**** Construct Prefix Tree....\n");
	while (!feof(fp)) {
		fgets(strline, MAX_LINE, fp);				    

		line_split(strline, str_ip, str_mask, str_port);
		str2uint(str_ip, str_mask, str_port, &ip, &mask, &port);
        
        IP_Database[IP_count] = ip;
        Mask_Database[IP_count] = mask;
        Port_Database[IP_count] = port;

        Node_t *node = (Node_t *)malloc(sizeof(Node_t));
        Memory_Cost += sizeof(Node_t);
        init_node(node, ip, mask, port);   
        
        insert_node(node);

        IP_count++;
        // printf("**** Add %d Node Into Tree\n", IP_count);
	}
	//关闭文件 
	fclose(fp);	
    printf("**** Construct Prefix Tree Succeed\n");

    return 0;
}

// flag == 0 : left, flag == 1 : right
void insert_node(Node_t *node)
{
    u32 ip = node->ip;
    short prefix = node->prefix;
    Node_t *temp = root, *parent = NULL, *q = NULL;

	u32 flag;
	while (temp != NULL && prefix > 0)
    {
        flag = (prefix == 1) ? ip & MOSTSIGBIT : ip & MOSTSIGBITS;
		parent = temp;

        if(flag == MOST00)
            temp = temp->child00;
        else if(flag == MOST01)
            temp = temp->child01;
        else if(flag == MOST10)
            temp = temp->child10;
        else if(flag == MOST11)
            temp = temp->child11;

		ip <<= 2;
		prefix -= 2;
	}

	if(prefix <= 0) 
    {
        // 如果后面没有节点，把node挂上去
		if (!temp)
        {
            if(flag == MOST00)
                parent->child00 = node;
            else if(flag == MOST01)
                parent->child01 = node;
            else if(flag == MOST10)
                parent->child10 = node;
            else if(flag == MOST11)
                parent->child11 = node;
		}
        // 如果有节点，修改节点信息
		else 
        {
			temp->prefix = node->prefix;
            if(prefix == 0)
                temp->even_port = node->even_port;
            else
			    temp->odd_port = node->odd_port;

			free(node);
            Memory_Cost -= sizeof(Node_t);
		}
	}
	else
    {
		while(prefix > 0)
        {
            q = (Node_t *)malloc(sizeof(Node_t));
            Memory_Cost += sizeof(Node_t);
            q->child00 = q->child01 = q->child10 = q->child11 = NULL;

            if(flag == MOST00)
                parent->child00 = q;
            else if(flag == MOST01)
                parent->child01 = q;
            else if(flag == MOST10)
                parent->child10 = q;
            else if(flag == MOST11)
                parent->child11 = q;

            parent = q;

            flag = (prefix == 1) ? ip & MOSTSIGBIT : ip & MOSTSIGBITS;
			
            ip <<= 2;
            prefix -= 2;
		}

        if(flag == MOST00)
            parent->child00 = node;
        else if(flag == MOST01)
            parent->child01 = node;
        else if(flag == MOST10)
            parent->child10 = node;
        else if(flag == MOST11)
            parent->child11 = node;
	}

}

Node_t *look_up_ip(u32 ip, u16 mask)
{
    short my_mask = (short)mask;
    u32 flag;
    Node_t *node = root, *parent = NULL;

    while(node && my_mask >= -1) {
        flag = ip & MOSTSIGBITS; 
        parent = node;

        if(flag == MOST00)
            node = node->child00;
        else if(flag == MOST01)
            node = node->child01;
        else if(flag == MOST10)
            node = node->child10;
        else if(flag == MOST11)
            node = node->child11;

        my_mask -= 2;
        ip <<= 2;
    }

    return parent;

}

void look_up_one()
{
	char str_ip[STRIPLEN];
    char str_mask[STRMASKLEN];
	while(1) {
		printf("Input IP and Mask --- eg. (1.0.4.0 24)\n>>>> ");
		scanf("%s %s", str_ip, str_mask);

		u32 ip = ip_str2uint(str_ip);
        u16 mask = mask_str2uint(str_mask);
		printf(">>>> Looking Up IP: "IP_FMT", Mask: %u\n", HOST_IP_FMT_STR(ip), mask);
		
		Node_t *result = look_up_ip(ip, mask);

		printf("\n=================Search Result=================\n");
		if(result) {
			printf(">>>> Netowrk: "IP_FMT"\n", HOST_IP_FMT_STR(result->ip));
			printf(">>>> Prefix: %d\n", result->prefix);
			printf(">>>> Port: %d\n", mask % 2 ? result->odd_port : result->even_port);
		}
		else
			printf("IP Not Found\n");

		printf("\n");
	}    
}

int look_up_all()
{
    struct  timeval start, end;

    Node_t *result;
    u32 error_count = 0;
    
    gettimeofday(&start, NULL);
    for(int i = 0; i < IP_count; i++)
        if((result = look_up_ip(IP_Database[i], Mask_Database[i])))
            Port_Results[i] = Mask_Database[i] % 2 ? result->odd_port : result->even_port;
        else
        {
            Port_Results[i] = -1;
            error_count++;
        }

    gettimeofday(&end, NULL);
    
    double time = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    printf("\n============================================\n");
    printf(">>>> Look Up IP Done, Error %u times\n", error_count);
    printf(">>>> Totaly Time Cost %fus\n", time);
    printf(">>>> Avarange Cost %fus Per Entry\n", time / IP_count);
    printf("++++ Memory Cost %u Bytes\n", Memory_Cost);
    printf("\n");

    return 0;   
}

int check_results()
{
    u32 error_count = 0;
    printf("\n================Check Results===============\n");    
    for(int i = 0; i < IP_count; i++)
    {
        if(Port_Results[i] != Port_Database[i]) {
            printf(">>>> The %dth Entry Error!!!, IP: "IP_FMT"\n", i, HOST_IP_FMT_STR(IP_Database[i]));
            printf(">>>> Result: %u, Trace: %u\n\n", Port_Results[i], Port_Database[i]);
            error_count++;
        }
    }

    if(error_count)
        printf(">>>> Total Error %u Entry\n", error_count);
    else 
        printf(">>>> All Results Right\n");

    return 0;
}