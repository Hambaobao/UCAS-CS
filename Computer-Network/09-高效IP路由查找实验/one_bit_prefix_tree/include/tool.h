#include <stdio.h>
#include <stdlib.h>
#include "type.h"

#define MAX_LINE 50
#define STRIPLEN 40
#define STRMASKLEN 4
#define STRPORTLEN 10

u16 mask_str2uint(const char *str_mask);
u16 port_str2uint(const char *str_port);
u32 ip_str2uint(const char *str_ip);
void line_split(const char *strline, char *str_ip, char *str_mask, char *str_port);
void str2uint(const char *str_ip, const char *str_mask, const char *str_port,
		 	u32 *ip, u16 *mask, u16 *port);