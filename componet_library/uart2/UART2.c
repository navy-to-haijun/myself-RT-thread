
#include <UART2.h>

/*���ţ�UART2��RX=PA3��TX=PA2*/

/*���ܣ�UART2�жϽ��գ���ѯ����*/


#define MYSELF_UART_NAME  "uart2"

#define BEEP    GET_PIN(D, 2)
#define KEY0    GET_PIN(B, 4)
#define KEY1    GET_PIN(B, 3)

static rt_device_t serial;									 //���崮���豸���
static  struct serial_configure uart_config={
    BAUD_RATE_115200, 		/*������115200*/
    DATA_BITS_8,	  			/*����λ 8*/
     STOP_BITS_1,	  			/*ֹͣλ 8*/
    PARITY_NONE,     		 /* ����żУ��*/
    BIT_ORDER_LSB,   		 /* ��λ��ǰ */ 
    NRZ_NORMAL, 	 			 /* normal mode */
   128,				  				/*�������Ĵ�С��128*/
    0 				  				 /* ����λ */
	};  //���ڲ������ýṹ��

static struct rt_semaphore  rt_urat_sem;		//��������ź���

/*���ջص�����*/
static rt_err_t  uart_input(rt_device_t dev,rt_size_t size)
{
	rt_sem_release(&rt_urat_sem);		//�ͷ��ź�����������һ
	return RT_EOK;
}
/*�����̺߳���*/
static void serial_thread_entry(void *parameter)
{
	char ch;
	while(1)
	{
		while(rt_device_read(serial,0,&ch,1)!=1)	//�ж��Ƿ�������
		{
			rt_sem_take(&rt_urat_sem,RT_WAITING_FOREVER);	//û�����ݾ͹���
		}
		rt_device_write(serial,0,&ch,1);	//����
		rt_kprintf("%c",ch);
		if(ch=='!')
		{
			rt_kprintf("\n",ch);
			rt_device_close(serial);
			break;
		}
			
	}
}
/*�����߳�:Ĭ��uart2�����Σ�ʹ�ò���*/
static int  uart_sample(int argc,char **argv)
{
	rt_uint8_t uart_MAX=6;
	char uart_name[uart_MAX];
	char str[]="hello RT thread!\n";
	rt_err_t flag;
	if(argc==2)
	{
		rt_strncpy(uart_name,argv[1],uart_MAX);
	}
	else{
		rt_strncpy(uart_name,MYSELF_UART_NAME,uart_MAX);
	}
	
	/*���Ҵ����豸*/
	serial=rt_device_find(uart_name);	
	if(!serial)
	{
		rt_kprintf("find %s failed!\n", uart_name);
		return RT_ERROR;
	}
	else{
		rt_kprintf("find %s OK!\n", uart_name);
	}
	/*���ƴ����豸�� */
	rt_device_control(serial,RT_DEVICE_CTRL_CONFIG,&uart_config);
	/*��ʼ���ź���*/
	rt_sem_init(&rt_urat_sem,"rt_sem",0,RT_IPC_FLAG_FIFO);
	/*�򿪴����豸�����жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
	flag=rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
	if(flag==RT_EOK)
			rt_kprintf("open!\n");
	else rt_kprintf("Not open!\n");
	/*���ûص�����*/
	rt_device_set_rx_indicate(serial,uart_input);
	/*�����ַ���*/
	rt_device_write(serial,0,str,sizeof(str)-1);
	/*�����߳�*/

	rt_thread_t thread=rt_thread_create("serial",serial_thread_entry,
																			RT_NULL,1024,20,10);
	if(thread!=RT_NULL )
	{
		rt_thread_startup(thread);
	}
	else{return RT_ERROR;}
	return RT_EOK;
}

MSH_CMD_EXPORT(uart_sample, urat sample:uart);





void beep_on(void *args)
{
    rt_kprintf("turn on beep!\n");

    rt_pin_write(BEEP, PIN_HIGH);
}

void beep_off(void *args)
{
    rt_kprintf("turn off beep!\n");

    rt_pin_write(BEEP, PIN_LOW);
}

static void pin_beep_sample(void)
{
    /* ����������Ϊ���ģʽ */
    rt_pin_mode(BEEP, PIN_MODE_OUTPUT);
    /* Ĭ�ϵ͵�ƽ */
    rt_pin_write(BEEP, PIN_HIGH);

    /* ����0����Ϊ����ģʽ */
    rt_pin_mode(KEY0, PIN_MODE_INPUT_PULLUP);
    /* ���жϣ��½���ģʽ���ص�������Ϊbeep_on */
    rt_pin_attach_irq(KEY0, PIN_IRQ_MODE_FALLING, beep_on, RT_NULL);
    /* ʹ���ж� */
    rt_pin_irq_enable(KEY0, PIN_IRQ_ENABLE);

    /* ����1����Ϊ����ģʽ */
    rt_pin_mode(KEY1, PIN_MODE_INPUT_PULLUP);
    /* ���жϣ��½���ģʽ���ص�������Ϊbeep_off */
    rt_pin_attach_irq(KEY1, PIN_IRQ_MODE_FALLING, beep_off, RT_NULL);
    /* ʹ���ж� */
    rt_pin_irq_enable(KEY1, PIN_IRQ_ENABLE);
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(pin_beep_sample, pin beep sample);





















