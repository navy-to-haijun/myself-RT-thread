/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-21     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "drv_lcd.h"
#include "touch.h"

#include <TIM3.h>
#include <lvgl.h>
#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "lv_apps\demo\demo.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>



int main(void)
{
    TIM3_init();                        //1ms心跳节拍
    LCD_Init();
    LCD_Clear(WHITE);
    tp_dev.init();                      //触摸屏初始化

    lv_init();                          //lvgl初始化
    lv_port_disp_init();                //显示驱动初始化
    lv_port_indev_init();               //触摸驱动初始化



    demo_create();
    while(1)
        {
            tp_dev.scan(0);
            lv_task_handler();
        }

}
