#include "type.h"
#include <stdio.h>
#include <stdlib.h>

u16 mask_str2uint(const char *str_mask)
{
	return (u16)atoi(str_mask); 
}

u16 port_str2uint(const char *str_port)
{
	return (u16)atoi(str_port); 
}

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

void line_split(const char *strline, char *str_ip, char *str_mask, char *str_port)
{
	int i = 0, j;
	for(j = 0; strline[i] != ' '; i++, j++)
		str_ip[j] = strline[i];
	str_ip[j] = '\0';
	i++;

	for(j = 0; strline[i] != ' '; i++, j++)
		str_mask[j] = strline[i];
	str_mask[j] = '\0';
	i++;

	for(j = 0; strline[i] != '\0'; i++, j++)
		str_port[j] = strline[i];
	str_port[j] = '\0';	

	// printf("%s %s %s", str_ip, str_mask, str_port);	
}

void str2uint(const char *str_ip, const char *str_mask, const char *str_port,
		 	u32 *ip, u16 *mask, u16 *port)
{
	*ip = ip_str2uint(str_ip);
	*mask = mask_str2uint(str_mask);
	*port = port_str2uint(str_port);

	// printf("%u %u %u\n", *ip, *mask, *port);	 
}