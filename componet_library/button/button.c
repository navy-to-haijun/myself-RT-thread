#include <button.h>
/*ʵ�ְ������ܣ��������ְ����Ķ̰�������*/
#define MY_BUTTON_CALL(func, argv) \
    do { if ((func) != RT_NULL) func argv; } while (0)

/*��ʼ��������Ϣ*/
int my_button_register(struct my_button *button)
{
	if(button->press_logic_level==0)	//��������Ϊ�͵�ƽ
	{
		rt_pin_mode(button->pin,PIN_MODE_INPUT_PULLUP);

	}
	else{
		rt_pin_mode(button->pin,PIN_MODE_INPUT_PULLDOWN);
	}
	button->cnt=0;										 //����ɨ�赽����״̬�Ĵ�����ʼΪ��
	button->event=BUTTON_EVENT_NONE;	//��ʼ��״̬
	 /* ��Ӱ����������б� */
	button_manage.button_list[button_manage.num++]=button;
	return 0;
}
/*����ɨ�躯��*/
static void my_button_scan(void *param)
{
	rt_uint8_t i;
	rt_uint16_t cnt_old;	
	for(i=0;i<button_manage.num;i++)
	{
		cnt_old=button_manage.button_list[i]->cnt;	//��ȡÿ�������İ��´���
		/* ��ⰴ���ĵ�ƽ״̬Ϊ����״̬ */
		if(rt_pin_read(button_manage.button_list[i]->pin)==button_manage.button_list[i]->press_logic_level)
		{
			button_manage.button_list[i]->cnt ++;		//����ɨ��Ĵ�����һ
			
			/* �������µ�ʱ��ﵽ���������¼���������ֵ */
			if (button_manage.button_list[i]->cnt == MY_BUTTON_DOWN_MS / MY_BUTTON_SCAN_SPACE_MS)
			{
				button_manage.button_list[i]->event = BUTTON_EVENT_CLICK_DOWN;
				MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));	//���ûص����� 
			}
			/* �������µ�ʱ��ﵽ������ʼ�¼���������ֵ */
			else if(button_manage.button_list[i]->cnt==MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS)
			{
				button_manage.button_list[i]->event = BUTTON_EVENT_HOLD;
				MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
			}
			/* �������µ�ʱ��ﵽ�������ڻص��¼���������ֵ */
			else if(button_manage.button_list[i]->cnt > MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS)
			{
				button_manage.button_list[i]->event = BUTTON_EVENT_HOLD_CYC;
				 if (button_manage.button_list[i]->hold_cyc_period && button_manage.button_list[i]->cnt % (button_manage.button_list[i]->hold_cyc_period / MY_BUTTON_SCAN_SPACE_MS) == 0)
              MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
			}
		}
		 /* ��ⰴ���ĵ�ƽ״̬Ϊ̧��״̬ */
		else{
			/* ��������ļ���ֵ */
       button_manage.button_list[i]->cnt = 0;
       /* �������µ�ʱ��ﵽ���������¼���������ֵ */
       if (cnt_old >= MY_BUTTON_DOWN_MS / MY_BUTTON_SCAN_SPACE_MS && cnt_old < MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS) /* BUTTON_CLICK_UP */
       {
         button_manage.button_list[i]->event = BUTTON_EVENT_CLICK_UP;
          MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
        }
         /* �������µ�ʱ��ﵽ���������¼���������ֵ */
        else if (cnt_old >= MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS) /* BUTTON_HOLD_UP */
        {
						button_manage.button_list[i]->event = BUTTON_EVENT_HOLD_UP; 
            MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
        }
		}
	}		
}
/*��ʼ����ʱ��*/
int my_button_start()
{
    if (button_manage.timer != RT_NULL)
        return -1;

    /* ������ʱ��1 */
    button_manage.timer = rt_timer_create("timer1", /* ��ʱ�������� timer1 */
                             my_button_scan, /* ��ʱʱ�ص��Ĵ����� */
                             RT_NULL, /* ��ʱ��������ڲ��� */
                              MY_BUTTON_SCAN_SPACE_MS, /* ��ʱ���ȣ���OS TickΪ��λ����10��OS Tick */
                             RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER); /* �����Զ�ʱ�� */
    /* ������ʱ�� */
    if (button_manage.timer != RT_NULL)
        rt_timer_start(button_manage.timer);
    return 0;
}


	/*���������¼�*/
int button_event_up(void)
{
	rt_kprintf("button event up\n");
	return  0;
}
/*���������Դ����¼�*/
int button_hold_cyc()
{
	rt_kprintf("button event hold cyc\n");
	return 0;
}

/*�����ص�����*/
void key_cb(struct my_button *button)
{
	switch(button->event)
	{
		case BUTTON_EVENT_CLICK_UP:button_event_up();					break;
		case BUTTON_EVENT_HOLD_CYC:button_hold_cyc();					break ;
		default:   ; 
	}
}

/*�����ص�����*/
void key_cb1(struct my_button *button)
{
	switch(button->event)
	{
		case BUTTON_EVENT_CLICK_UP:{rt_kprintf("�¼���������\n");}			break;
		case BUTTON_EVENT_HOLD_CYC:{rt_kprintf("�¼����ڻص���������\n");};					break ;
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
    my_button_register(&key1);			//ע�ᰴ��
		my_button_register(&key2);			//ע�ᰴ��
    my_button_start();						//��ʼ����ʱ��������ɨ��
}


MSH_CMD_EXPORT(key_fun, key);
