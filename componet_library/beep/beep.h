#ifndef BEEP_H
#define BEEP_H

#include <rtdevice.h>
#include <rtthread.h>
#include <drv_gpio.h>

#define BEEP_PWM_DEVICE  "pwm3"
#define BEEP_PWM_CH      4

int beep_init(void);                         //��������ʼ��
int beep_on(void);                           //��������
int beep_off(void);                          //��������
int beep_set(uint16_t freq, uint8_t volume); //�������趨
#endif

