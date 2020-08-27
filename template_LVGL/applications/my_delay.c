#include <my_delay.h>
#include "rtdevice.h"
#include <drv_common.h>
void delay_us(rt_uint32_t us)
{
    rt_hw_us_delay(us);
}
void delay_ms(rt_int32_t ms)
{
    rt_thread_mdelay(ms);
}


