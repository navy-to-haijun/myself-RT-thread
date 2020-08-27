#ifndef BEEP_H
#define BEEP_H

#include <rtdevice.h>
#include <rtthread.h>
#include <drv_gpio.h>

#define BEEP_PWM_DEVICE  "pwm3"
#define BEEP_PWM_CH      4

int beep_init(void);                         //蜂鸣器初始化
int beep_on(void);                           //蜂鸣器开
int beep_off(void);                          //蜂鸣器关
int beep_set(uint16_t freq, uint8_t volume); //蜂鸣器设定
#endif

