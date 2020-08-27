#include <beep.h>

struct rt_device_pwm *pwm_device = RT_NULL; //���� pwm �豸ָ��

/*��������ʼ��*/
int beep_init(void)
{
	/* ����PWM�豸 */
  pwm_device = (struct rt_device_pwm *)rt_device_find(BEEP_PWM_DEVICE);
	if (pwm_device == RT_NULL)
    {
        rt_kprintf("pwm device %s not found!\n", BEEP_PWM_DEVICE);
        return -RT_ERROR;
    }
	return  0;
}
/*�򿪷�����*/
int beep_on(void)
{
    rt_pwm_enable(pwm_device, BEEP_PWM_CH); //ʹ�ܷ�������Ӧ�� PWM ͨ��
    return 0;
}
/*�رշ�����*/
int beep_off(void)
{
    rt_pwm_disable(pwm_device, BEEP_PWM_CH); //ʧ�ܷ�������Ӧ�� PWM ͨ��
    return 0;
}
/*�������趨
	���룺Ƶ�ʣ�������0��100
*/
int beep_set(uint16_t freq, uint8_t volume)
{
	  rt_uint32_t period, pulse;
    /* ��Ƶ��ת��Ϊ���� ���ڵ�λ:ns Ƶ�ʵ�λ:HZ */
    period = 1000000000 / freq; 
    /* ����������С����ռ�ձ� �������͵�ƽ���� */
    pulse = period - period / 100 * volume;
    /* ���� PWM API �趨 ���ں�ռ�ձ� */
    rt_pwm_set(pwm_device, BEEP_PWM_CH, period, pulse);//channel,period,pulse
    return 0;
}