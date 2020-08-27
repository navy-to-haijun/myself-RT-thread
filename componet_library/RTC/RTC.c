#include <RTC.h>

static int rtc_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    time_t now;
	
    /* �������� */
	  ret = set_date(2020, 7, 19);
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC date failed\n");
        return ret;
    }

    /* ����ʱ�� */
    ret = set_time(17, 30, 0);
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC time failed\n");
        return ret;
    }

    /* ��ʱ3�� */
    rt_thread_mdelay(3000);

    /* ��ȡʱ�� */
    now = time(RT_NULL);
    rt_kprintf("%s\n", ctime(&now));

    return ret;
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(rtc_sample, rtc sample);