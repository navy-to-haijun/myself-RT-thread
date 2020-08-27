#ifndef BUTTON_H
#define BUTTON_H

#include <rtdevice.h>
#include <rtthread.h>
#include <drv_gpio.h>

#define MY_BUTTON_DOWN_MS        50
#define MY_BUTTON_HOLD_MS        700
#define MY_BUTTON_SCAN_SPACE_MS  20

#define MY_BUTTON_LIST_MAX 10

typedef void (*my_button_callback)(void*);		//函数指针别名

/*枚举按键事件:第一个枚举成员的默认值为整型的 0*/
enum my_button_event
{
	BUTTON_EVENT_CLICK_DOWN=0,		//单击按下事件
	BUTTON_EVENT_CLICK_UP,	  	//单击结束事件
	BUTTON_EVENT_HOLD,					//按键长按开始事件
	BUTTON_EVENT_HOLD_CYC,			//长按周期性触发事件
	BUTTON_EVENT_HOLD_UP,				//按键长按结束事件
	BUTTON_EVENT_NONE,					//无按键事件
};
/*管理按键信息*/
struct my_button
{
	rt_uint8_t press_logic_level;		//按键按下时的电平
	rt_uint16_t cnt;								//连续扫描到按下状态的次数
	rt_uint16_t hold_cyc_period;		//长按周期回调的周期
	rt_uint16_t pin;								//按键对应的 pin 编号
	enum my_button_event event;			//按键的触发的事件
  my_button_callback cb;
};
#endif
/*存储按键信息*/
struct my_button_manage
{
	rt_uint8_t num;                 /* 已注册的按键的数目 */
  rt_timer_t timer;               /* 按键扫描用到的定时器 */
  struct my_button *button_list[MY_BUTTON_LIST_MAX];  /* 存储按键指针的数组 */
};
struct my_button_manage button_manage;
