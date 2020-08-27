/*

 */
#include "LED.h"

#define LED0_PIN    GET_PIN(E, 5)
#define LED0_PIN    GET_PIN(E, 5)

int LED_Test(void)
{
    int count = 1;
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (count++)
    {
        rt_pin_write(LED0_PIN, count % 2);
        rt_thread_mdelay(1000);
    }
    return RT_EOK;
}

MSH_CMD_EXPORT(LED_Test, LED);
