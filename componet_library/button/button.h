#ifndef BUTTON_H
#define BUTTON_H

#include <rtdevice.h>
#include <rtthread.h>
#include <drv_gpio.h>

#define MY_BUTTON_DOWN_MS        50
#define MY_BUTTON_HOLD_MS        700
#define MY_BUTTON_SCAN_SPACE_MS  20

#define MY_BUTTON_LIST_MAX 10

typedef void (*my_button_callback)(void*);		//����ָ�����

/*ö�ٰ����¼�:��һ��ö�ٳ�Ա��Ĭ��ֵΪ���͵� 0*/
enum my_button_event
{
	BUTTON_EVENT_CLICK_DOWN=0,		//���������¼�
	BUTTON_EVENT_CLICK_UP,	  	//���������¼�
	BUTTON_EVENT_HOLD,					//����������ʼ�¼�
	BUTTON_EVENT_HOLD_CYC,			//���������Դ����¼�
	BUTTON_EVENT_HOLD_UP,				//�������������¼�
	BUTTON_EVENT_NONE,					//�ް����¼�
};
/*��������Ϣ*/
struct my_button
{
	rt_uint8_t press_logic_level;		//��������ʱ�ĵ�ƽ
	rt_uint16_t cnt;								//����ɨ�赽����״̬�Ĵ���
	rt_uint16_t hold_cyc_period;		//�������ڻص�������
	rt_uint16_t pin;								//������Ӧ�� pin ���
	enum my_button_event event;			//�����Ĵ������¼�
  my_button_callback cb;
};
#endif
/*�洢������Ϣ*/
struct my_button_manage
{
	rt_uint8_t num;                 /* ��ע��İ�������Ŀ */
  rt_timer_t timer;               /* ����ɨ���õ��Ķ�ʱ�� */
  struct my_button *button_list[MY_BUTTON_LIST_MAX];  /* �洢����ָ������� */
};
struct my_button_manage button_manage;
