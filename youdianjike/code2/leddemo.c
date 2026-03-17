#include <stdio.h>
#include "ohos_init.h"
#include <unistd.h>
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "ohos_types.h"

void leddemo(void)
{
    GpioInit(); //初始化GPIO设备
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_IO_FUNC_GPIO_9_GPIO);//设置GPIO9为普通IO功能
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_GPIO_DIR_OUT);//设置GPIO9为输出方向
    
    GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_GPIO_VALUE0);//输出低电平，点亮LED
    printf("led open \n");

    usleep(4000000);//点亮4秒
    GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_GPIO_VALUE1);//输出高电平，熄灭LED
    printf("led close \n"); 
}
SYS_RUN(leddemo);