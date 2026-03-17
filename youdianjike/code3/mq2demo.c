#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_adc.h"
#include "wifiiot_errno.h"

#define GAS_SENSOR_CHAN_NAME WIFI_IOT_ADC_CHANNEL_5//传感器ADC通道

// 传感器数据读取任务函数
static void Mq2DemoTask(void *arg)
{
    (void)arg;
    GpioInit();//初始化GPIO设备
    while (1)
    {
        unsigned short data = 0;//存储传感器数据
        // 读取ADC数据，12位分辨率，1.2V参考电压
        if (AdcRead(GAS_SENSOR_CHAN_NAME, &data, WIFI_IOT_ADC_EQU_MODEL_4, WIFI_IOT_ADC_CUR_BAIS_DEFAULT, 0) == WIFI_IOT_SUCCESS)
        {
            printf("qi wei value:%d\n", data);
        }
        sleep(1);//每秒读取1次
    }
}

// 创建线程并初始化
static void Mq2DemoTaskEntry(void)
{
    osThreadAttr_t attr;
    attr.name = "Mq2DemoTask";//线程名称
    attr.attr_bits = 0;
    attr.cb_mem = NULL;
    attr.cb_size = 0;
    attr.stack_mem = NULL;
    attr.stack_size = 2048;//栈大小
    attr.priority = osPriorityNormal;//线程优先级

    // 创建线程
    if (osThreadNew(Mq2DemoTask, NULL, &attr) == NULL)
    {
        printf("Failed to create Mq2DemoTask!\n");
    }
}
// 应用初始化，执行线程任务
APP_FEATURE_INIT(Mq2DemoTaskEntry);