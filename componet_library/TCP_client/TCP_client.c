#include <TCP_client.h>

/*�������ݽ���*/
void weather_analyis(struct at_client *client,const char *data, rt_size_t size)
{
	rt_kprintf("����");
	rt_kprintf("�������ݣ�%s\n",data);
}
/*ʱ�����ݽ���*/
void time_analyis(struct at_client *client,const char *data, rt_size_t size)
{
	rt_kprintf("ʱ�����ݣ�%s\n",data);
}

static struct at_urc urc_table[] = {
    {"{\"sysTime2",   "}",     weather_analyis},
    {"{\"results\":",     "}",       time_analyis},
};

void TCP_URC_init(void)
{
	
}

at_client_t client = RT_NULL;
int TCP_client_init(void)
{
	/*��ʼ��AT�ͻ���*/
	at_client_init (AT_SERVER_DEVICE,AT_SERVER_RECV_BUFF_LEN);
	at_set_urc_table(urc_table, sizeof(urc_table) / sizeof(urc_table[0]));
	/* ͨ�����ƻ�ȡ��Ӧ�� AT �ͻ��˶��� */
  client = at_client_get("uart3");
	at_client_send("+++",3);
	rt_thread_mdelay(100);
	return RT_EOK;
}

int TCP_send_AT(const char *commend,rt_size_t struct_buff)
{
	at_response_t resp = RT_NULL;		//������Ӧ�ṹ��
	rt_uint8_t i;
	char at_receive[100]={"\n"};
	
	/*������Ӧ�ṹ�壺���ݳ��ȣ�struct_buff�ֽڣ����������ƣ���ʱʱ�䣺10S*/
	resp=at_create_resp(struct_buff,0,rt_tick_from_millisecond(5000));
	if(!resp)
	{
		rt_kprintf("��Ӧ�ṹ�崴��ʧ��\r\n");
		return -1;
	}
	rt_kprintf("��Ӧ�ṹ�崴���ɹ�\r\n");
	/* ���� AT ������� AT Server ��Ӧ���ݣ����ݼ���Ϣ����� resp �ṹ���� */
	rt_kprintf("���͵�ATָ�%s\r\n",commend);
	if(at_exec_cmd(resp,commend)<0)
	{
		rt_kprintf("ATָ���ʧ��,�����ٴη���\r\n");
		if(at_exec_cmd(resp,commend)<0)
		{
			rt_kprintf("ATָ���ʧ��\r\n");
			return -2;
		}	
	}
	rt_kprintf("ATָ��ͳɹ�\r\n");
	/*������Ӧ����:ץȡ�ؼ���*/
	  if(at_resp_parse_line_args(resp,1,"%s\n",at_receive)<=0)
		{
			rt_kprintf("��������ʧ��!\n");
			return -3;
		}
	/*��ʾ��Ӧ����*/
	for(i=1;i<=resp->line_counts;i++)
	{
		rt_kprintf("��%d�����ݣ�%s\r\n",i,at_resp_get_line(resp,i));
	}
	at_delete_resp(resp);
	rt_thread_mdelay(100);
	return RT_EOK;
}

int link_wifi(void)
{
	char *AT_commend[]={
	"AT",
	"AT+CWMODE=1",
	"AT+RST",
	"AT+CWJAP=\"FAST_93B0\",\"13550789085\"",
	"AT+CIPMUX=0",
	};
	
	rt_uint8_t i;
	for(i=0;i<sizeof(AT_commend)/sizeof(AT_commend[0]);i++)
	{
		if(TCP_send_AT(AT_commend[i],1024)!=RT_EOK)
		{
			rt_kprintf("����%s���ô���\n",AT_commend[i]);
			return 0;
		}
		rt_kprintf("����%s������ȷ\n",AT_commend[i]);
	}
	
	return RT_EOK ;
}

int get_time(void)
{

	char *AT_commend[]={
	"AT+CIPSTART=\"TCP\",\"http://quan.suning.com\",80",
//	"AT+CIPSTART=\"TCP\",\"192.168.0.104\",8086",
	"AT+CIPMODE=1",
	"AT+CIPSEND",
	};
	char API[]={"GET http://quan.suning.com/getSysTime.do\r\n"};
	rt_int8_t i;
	at_client_send("+++",3);
	rt_thread_mdelay(100);
	for(i=0;i<sizeof(AT_commend)/sizeof(AT_commend[0]);i++)
	{
		if(TCP_send_AT(AT_commend[i],1028)!=RT_EOK)
		{
			rt_kprintf("�����ô���\n");
			return 0;
		}
		rt_kprintf("����������ȷ\n");
		
		at_set_end_sign('}');
	}
	if(at_client_send(API,sizeof(API))<=0)
	{
		rt_kprintf("ʱ�� API ����ʧ��\n");
	}
	rt_kprintf("ʱ�� API ���ͳɹ�\n");
	for(i=1;i<10;i++)
	{
		at_client_send(API,sizeof(API));
		rt_kprintf("%d\n",i);
		rt_thread_mdelay(2000);
	}
	return RT_EOK;
}
int get_weather(void)
{
	char *AT_commend[]={
//	"AT+CIPSTART=\"TCP\",\"api.seniverse.com\",80",
	"AT+CIPSTART=\"TCP\",\"192.168.0.104\",8086",
	"AT+CIPMODE=1",
	"AT+CIPSEND",
	};
	char API[]={"GET https://api.seniverse.com/v3/weather/now.json?key=Sy4GgsLmyKTO4Orj7&location=WM7JD7CXZK37&language=zh-Hans&unit=c\r\n"};
	rt_int8_t i;
	at_client_send("+++",3);
	rt_thread_mdelay(100);
	for(i=0;i<sizeof(AT_commend)/sizeof(AT_commend[0]);i++)
	{
		if(TCP_send_AT(AT_commend[i],1028)!=RT_EOK)
		{
			rt_kprintf("�����ô���\n");
			return 0;
		}
		rt_kprintf("����������ȷ\n");
	}
	at_set_end_sign('}');
	if(at_client_send(API,sizeof(API))<=0)
	{
		rt_kprintf("���� API ����ʧ��\n");
	}
	rt_thread_mdelay(1000);
	for(i=1;i<5;i++)
	{
		at_client_send(API,sizeof(API));
		rt_kprintf("%d\n",i);
		rt_thread_mdelay(1000);
	}
	return RT_EOK;
}


	
