#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <rtthread.h>
#include <at.h>



int TCP_client_init(void);
int TCP_send_AT(const char *commend,rt_size_t struct_buff);
int link_wifi(void);
int get_weather(void);
void TCP_URC_init(void);
int get_time(void);
	

#endif
