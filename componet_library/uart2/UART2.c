
#include <UART2.h>

/*引脚：UART2：RX=PA3，TX=PA2*/

/*功能：UART2中断接收，轮询发送*/


#define MYSELF_UART_NAME  "uart2"

#define BEEP    GET_PIN(D, 2)
#define KEY0    GET_PIN(B, 4)
#define KEY1    GET_PIN(B, 3)

static rt_device_t serial;									 //定义串口设备句柄
static  struct serial_configure uart_config={
    BAUD_RATE_115200, 		/*波特率115200*/
    DATA_BITS_8,	  			/*数据位 8*/
     STOP_BITS_1,	  			/*停止位 8*/
    PARITY_NONE,     		 /* 无奇偶校验*/
    BIT_ORDER_LSB,   		 /* 高位在前 */ 
    NRZ_NORMAL, 	 			 /* normal mode */
   128,				  				/*缓冲区的大小：128*/
    0 				  				 /* 保留位 */
	};  //串口参数配置结构体

static struct rt_semaphore  rt_urat_sem;		//定义接收信号量

/*接收回调函数*/
static rt_err_t  uart_input(rt_device_t dev,rt_size_t size)
{
	rt_sem_release(&rt_urat_sem);		//释放信号量，参数加一
	return RT_EOK;
}
/*接收线程函数*/
static void serial_thread_entry(void *parameter)
{
	char ch;
	while(1)
	{
		while(rt_device_read(serial,0,&ch,1)!=1)	//判断是否有数据
		{
			rt_sem_take(&rt_urat_sem,RT_WAITING_FOREVER);	//没有数据就挂起
		}
		rt_device_write(serial,0,&ch,1);	//发出
		rt_kprintf("%c",ch);
		if(ch=='!')
		{
			rt_kprintf("\n",ch);
			rt_device_close(serial);
			break;
		}
			
	}
}
/*接收线程:默认uart2，带参，使用参数*/
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
	
	/*查找串口设备*/
	serial=rt_device_find(uart_name);	
	if(!serial)
	{
		rt_kprintf("find %s failed!\n", uart_name);
		return RT_ERROR;
	}
	else{
		rt_kprintf("find %s OK!\n", uart_name);
	}
	/*控制串口设备。 */
	rt_device_control(serial,RT_DEVICE_CTRL_CONFIG,&uart_config);
	/*初始化信号量*/
	rt_sem_init(&rt_urat_sem,"rt_sem",0,RT_IPC_FLAG_FIFO);
	/*打开串口设备。以中断接收及轮询发送模式打开串口设备 */
	flag=rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
	if(flag==RT_EOK)
			rt_kprintf("open!\n");
	else rt_kprintf("Not open!\n");
	/*设置回调函数*/
	rt_device_set_rx_indicate(serial,uart_input);
	/*发送字符串*/
	rt_device_write(serial,0,str,sizeof(str)-1);
	/*创建线程*/

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
    /* 蜂鸣器引脚为输出模式 */
    rt_pin_mode(BEEP, PIN_MODE_OUTPUT);
    /* 默认低电平 */
    rt_pin_write(BEEP, PIN_HIGH);

    /* 按键0引脚为输入模式 */
    rt_pin_mode(KEY0, PIN_MODE_INPUT_PULLUP);
    /* 绑定中断，下降沿模式，回调函数名为beep_on */
    rt_pin_attach_irq(KEY0, PIN_IRQ_MODE_FALLING, beep_on, RT_NULL);
    /* 使能中断 */
    rt_pin_irq_enable(KEY0, PIN_IRQ_ENABLE);

    /* 按键1引脚为输入模式 */
    rt_pin_mode(KEY1, PIN_MODE_INPUT_PULLUP);
    /* 绑定中断，下降沿模式，回调函数名为beep_off */
    rt_pin_attach_irq(KEY1, PIN_IRQ_MODE_FALLING, beep_off, RT_NULL);
    /* 使能中断 */
    rt_pin_irq_enable(KEY1, PIN_IRQ_ENABLE);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(pin_beep_sample, pin beep sample);





















