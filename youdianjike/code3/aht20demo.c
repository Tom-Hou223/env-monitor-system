#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_i2c.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_errno.h"
#include "aht20.h"

#define AHT20_BAUDRATE 400 * 1000 // I2C波特率
#define AHT20_I2C_IDX WIFI_IOT_I2C_IDX_0 // I2C通道

// 初始化I2C与GPIO
void init(void)
{
    GpioInit();
    // 设置GPIO13为I2C0_SDA功能
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_13, WIFI_IOT_IO_FUNC_GPIO_13_I2C0_SDA);
    // 设置GPIO14为I2C0_SCL功能
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_14, WIFI_IOT_IO_FUNC_GPIO_14_I2C0_SCL);
    // 初始化I2C
    I2cInit(AHT20_I2C_IDX, AHT20_BAUDRATE);
}

// 温湿度读取任务函数
static void AhtDemoTask(void *arg){
    (void)arg;
    init();//调用设备初始化
    uint32_t retval = 0;//返回值
    float humidity = 0.0f;//湿度值
    float temperature = 0.0f;//温度值

    // AHT20传感器校准
    while (WIFI_IOT_SUCCESS != AHT20_Calibrate()){
        printf("AHT20 sensor init failed!\r\n");
        sleep(5);
    }
    // 循环读取温湿度数据
    while (1){
        retval = AHT20_StartMeasure();//触发测量
        printf("retval:%d\n", retval);
        if (retval != WIFI_IOT_SUCCESS){
            printf("trigger measure failed!\r\n");
            sleep(5);
        }else {
            // 获取测量结果
            retval = AHT20_GetMeasureResult(&temperature, &humidity);
            // 打印温湿度数据
            printf("temp: %.2f,  humi: %.2f\n", temperature, humidity);
        }
        sleep(1);//每秒读取1次
    }
}

// 创建线程函数并初始化
static void AhtDemoTaskEntry(void)
{
    osThreadAttr_t attr;
    attr.name = "AhtDemoTask";
    attr.attr_bits = 0;
    attr.cb_mem = NULL;
    attr.cb_size = 0;
    attr.stack_mem = NULL;
    attr.stack_size = 2048;
    attr.priority = osPriorityNormal;

    if (osThreadNew(AhtDemoTask, NULL, &attr) == NULL)
    {
        printf("Failed to create AhtDemoTask!\n");
    }
}
// 应用初始化
APP_FEATURE_INIT(AhtDemoTaskEntry);