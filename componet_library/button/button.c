#include <button.h>
/*实现按键功能，并能区分按键的短按，长按*/
#define MY_BUTTON_CALL(func, argv) \
    do { if ((func) != RT_NULL) func argv; } while (0)

/*初始化按键信息*/
int my_button_register(struct my_button *button)
{
	if(button->press_logic_level==0)	//按键按下为低电平
	{
		rt_pin_mode(button->pin,PIN_MODE_INPUT_PULLUP);

	}
	else{
		rt_pin_mode(button->pin,PIN_MODE_INPUT_PULLDOWN);
	}
	button->cnt=0;										 //连续扫描到按下状态的次数初始为零
	button->event=BUTTON_EVENT_NONE;	//初始无状态
	 /* 添加按键到管理列表 */
	button_manage.button_list[button_manage.num++]=button;
	return 0;
}
/*按键扫描函数*/
static void my_button_scan(void *param)
{
	rt_uint8_t i;
	rt_uint16_t cnt_old;	
	for(i=0;i<button_manage.num;i++)
	{
		cnt_old=button_manage.button_list[i]->cnt;	//获取每个按键的按下次数
		/* 检测按键的电平状态为按下状态 */
		if(rt_pin_read(button_manage.button_list[i]->pin)==button_manage.button_list[i]->press_logic_level)
		{
			button_manage.button_list[i]->cnt ++;		//按键扫描的次数加一
			
			/* 连续按下的时间达到单击按下事件触发的阈值 */
			if (button_manage.button_list[i]->cnt == MY_BUTTON_DOWN_MS / MY_BUTTON_SCAN_SPACE_MS)
			{
				button_manage.button_list[i]->event = BUTTON_EVENT_CLICK_DOWN;
				MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));	//调用回调函数 
			}
			/* 连续按下的时间达到长按开始事件触发的阈值 */
			else if(button_manage.button_list[i]->cnt==MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS)
			{
				button_manage.button_list[i]->event = BUTTON_EVENT_HOLD;
				MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
			}
			/* 连续按下的时间达到长按周期回调事件触发的阈值 */
			else if(button_manage.button_list[i]->cnt > MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS)
			{
				button_manage.button_list[i]->event = BUTTON_EVENT_HOLD_CYC;
				 if (button_manage.button_list[i]->hold_cyc_period && button_manage.button_list[i]->cnt % (button_manage.button_list[i]->hold_cyc_period / MY_BUTTON_SCAN_SPACE_MS) == 0)
              MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
			}
		}
		 /* 检测按键的电平状态为抬起状态 */
		else{
			/* 清除按键的计数值 */
       button_manage.button_list[i]->cnt = 0;
       /* 连续按下的时间达到单击结束事件触发的阈值 */
       if (cnt_old >= MY_BUTTON_DOWN_MS / MY_BUTTON_SCAN_SPACE_MS && cnt_old < MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS) /* BUTTON_CLICK_UP */
       {
         button_manage.button_list[i]->event = BUTTON_EVENT_CLICK_UP;
          MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
        }
         /* 连续按下的时间达到长按结束事件触发的阈值 */
        else if (cnt_old >= MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS) /* BUTTON_HOLD_UP */
        {
						button_manage.button_list[i]->event = BUTTON_EVENT_HOLD_UP; 
            MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
        }
		}
	}		
}
/*初始化定时器*/
int my_button_start()
{
    if (button_manage.timer != RT_NULL)
        return -1;

    /* 创建定时器1 */
    button_manage.timer = rt_timer_create("timer1", /* 定时器名字是 timer1 */
                             my_button_scan, /* 超时时回调的处理函数 */
                             RT_NULL, /* 超时函数的入口参数 */
                              MY_BUTTON_SCAN_SPACE_MS, /* 定时长度，以OS Tick为单位，即10个OS Tick */
                             RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER); /* 周期性定时器 */
    /* 启动定时器 */
    if (button_manage.timer != RT_NULL)
        rt_timer_start(button_manage.timer);
    return 0;
}


	/*单击结束事件*/
int button_event_up(void)
{
	rt_kprintf("button event up\n");
	return  0;
}
/*长按周期性触发事件*/
int button_hold_cyc()
{
	rt_kprintf("button event hold cyc\n");
	return 0;
}

/*按键回调函数*/
void key_cb(struct my_button *button)
{
	switch(button->event)
	{
		case BUTTON_EVENT_CLICK_UP:button_event_up();					break;
		case BUTTON_EVENT_HOLD_CYC:button_hold_cyc();					break ;
		default:   ; 
	}
}

/*按键回调函数*/
void key_cb1(struct my_button *button)
{
	switch(button->event)
	{
		case BUTTON_EVENT_CLICK_UP:{rt_kprintf("下键单击！！\n");}			break;
		case BUTTON_EVENT_HOLD_CYC:{rt_kprintf("下键周期回调函数！！\n");};					break ;
		default:   ; 
	}
}

void key_fun()
{
    static struct my_button key1 = {
							PIN_LOW,
							0,
							100,
							19,						/*PB3*/
							BUTTON_EVENT_NONE,
							(my_button_callback)key_cb,
		};
   static struct my_button key2 = {
							PIN_LOW,
							0,
							100,
							20,						/*PB4*/
							BUTTON_EVENT_NONE,
							(my_button_callback)key_cb1,
		};		
    my_button_register(&key1);			//注册按键
		my_button_register(&key2);			//注册按键
    my_button_start();						//初始化定时器，启动扫描
}


MSH_CMD_EXPORT(key_fun, key);
