#include "prefix_tree.h"
#include "tool.h"
#include "type.h"
#include <sys/time.h>

Node_t *root;

void init_node(Node_t *node, u32 ip, u16 prefix, u16 mask, u16 port) 
{
    node->ip = ip;
    node->prefix = prefix;
    node->mask = mask;
    node->port = port;
    node->left = NULL;
    node->right = NULL;
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

    init_node(root, 0, 0, 0, 0);

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

        init_node(node, ip, mask, mask, port);   
        
        insert_node(node);

        IP_count++;
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
    u16 prefix = node->prefix;
    Node_t *temp = root, *parent = NULL, *q = NULL;

	bool flag;
	while (temp != NULL && prefix > 0)
    {
		flag = ip & MOSTSIGBIT;
		parent = temp;
        temp = flag ? temp->right : temp->left; 

		ip <<= 1;
		prefix--;
	}

	if(prefix == 0) 
    {
        // 如果后面没有节点，把node挂上去
		if (!temp)
        {
            if(flag)
                parent->right = node;
            else 
                parent->left = node;
		}
        // 如果有节点，修改节点信息
		else 
        {
			temp->netowrk = node->netowrk;
			temp->prefix = node->prefix;
			temp->port = node->port;
			free(node);
            Memory_Cost -= sizeof(Node_t);
		}
	}
	else
    {
		while(prefix--)
        {
            q = (Node_t *)malloc(sizeof(Node_t));
            Memory_Cost += sizeof(Node_t);

            q->left = q->right = NULL;

            if(flag)
                parent->right = q;
            else 
                parent->left = q;

            parent = q;

			flag = ip & MOSTSIGBIT;
			ip <<= 1;
		}

        if(flag)
            parent->right = node;
        else 
            parent->left = node;
	}

}


Node_t *look_up_ip(u32 ip, u16 mask)
{
    short my_mask = mask;
    bool flag;
    Node_t *node = root, *parent = NULL;

    while(node && my_mask >= 0) {
        flag = ip & MOSTSIGBIT;
        
        parent = node;
        node = flag ? node->right : node->left;

        ip <<= 1;
        
        my_mask--;
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

		printf("\n================Search Result===============\n");
		if(result) {
			printf(">>>> Network: "IP_FMT"\n", HOST_IP_FMT_STR(result->ip));
			printf(">>>> Mask: %d\n", result->mask);
			printf(">>>> Prefix: %d\n", result->prefix);
			printf(">>>> Port: %d\n", result->port);
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
            Port_Results[i] = result->port;
        else
        {
            Port_Results[i] = -1;
            error_count++;
        }

    gettimeofday(&end, NULL);
    
    double time = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	
    printf("\n============================================\n");
    printf("**** Look Up IP Done, Error %u times\n", error_count);
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
            printf(">>>> The %dth Entry Error!!!\n", i);
            error_count++;
        }
    }

    if(error_count)
        printf(">>>> Total Error %u Entry\n", error_count);
    else 
        printf(">>>> All Results Right\n");

    return 0;
}