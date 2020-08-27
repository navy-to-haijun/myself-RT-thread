#include"PWM.h"

/*
 * �����嵥������һ�� PWM �豸ʹ������
 * ���̵����� pwm_led_sample ��������ն�
 * ������ø�ʽ��pwm_led_sample
 * �����ܣ�ͨ�� PWM �豸���� LED �Ƶ����ȣ����Կ���LED��ͣ���ɰ��䵽����Ȼ���ִ����䵽����
*/

//#include <rtthread.h>
//#include <rtdevice.h>

#define LED_PIN_NUM         17      /* LED PIN�ű�ţ��鿴�����ļ�drv_gpio.cȷ�� */

#define PWM_DEV_NAME        "pwm3"  /* PWM�豸���� */
#define PWM_DEV_CHANNEL     4       /* PWMͨ�� */

struct rt_device_pwm *pwm_dev;      /* PWM�豸��� */

static int pwm_led_sample(int argc, char *argv[])
{
    rt_uint32_t period, pulse, dir;

    period = 500000;    /* ����Ϊ0.5ms����λΪ����ns */
    dir = 1;            /* PWM������ֵ���������� */
    pulse = 0;          /* PWM������ֵ����λΪ����ns */

    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        return RT_ERROR;
    }

    /* ����PWM���ں�������Ĭ��ֵ */
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    /* ʹ���豸 */
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);

    while (1)
    {
        rt_thread_mdelay(50);
        if (dir)
        {
            pulse += 5000;      /* ��0ֵ��ʼÿ������5000ns */
        }
        else
        {
            pulse -= 5000;      /* �����ֵ��ʼÿ�μ���5000ns */
        }
        if (pulse >= period)
        {
            dir = 0;
        }
        if (0 == pulse)
        {
            dir = 1;
        }

        /* ����PWM���ں������� */
        rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    }
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(pwm_led_sample, pwm sample);
