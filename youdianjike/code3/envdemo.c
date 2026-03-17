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
#include "wifiiot_adc.h"
#include "oled_ssd1306.h"

#define GAS_SENSOR_CHAN_NAME WIFI_IOT_ADC_CHANNEL_5//传感器通道
#define AHT20_BAUDRATE 400 * 1000
#define AHT20_I2C_IDX WIFI_IOT_I2C_IDX_0

// 初始化I2C、GPIO、OLED
void init(void)
{
    GpioInit();
    OledInit();//初始化OLED
    // 配置I2C引脚
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_13, WIFI_IOT_IO_FUNC_GPIO_13_I2C0_SDA);
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_14, WIFI_IOT_IO_FUNC_GPIO_14_I2C0_SCL);
    I2cInit(AHT20_I2C_IDX, AHT20_BAUDRATE);
}

// 环境数据读取与显示任务
static void AhtDemoTask(void *arg){
    (void)arg;
    init();
    uint32_t retval = 0;
    unsigned short data = 0;//燃气传感器数据
    float humidity = 0.0f;
    float temperature = 0.0f;
    OledFillScreen(0x00);//清空屏幕

    // AHT20校准
    while (WIFI_IOT_SUCCESS != AHT20_Calibrate()){
        printf("AHT20 sensor init failed!\r\n");
        sleep(5);
    }
    // 循环读取数据
    while (1){
        retval = AHT20_StartMeasure();
        printf("retval:%d\n", retval);
        if (retval != WIFI_IOT_SUCCESS){
            printf("trigger measure failed!\r\n");
            sleep(5);
        }else {
            // 读取燃气传感器数据
            AdcRead(GAS_SENSOR_CHAN_NAME, &data, WIFI_IOT_ADC_EQU_MODEL_4, WIFI_IOT_ADC_CUR_BAIS_DEFAULT, 0);
            printf("qi wei value:%d\n", data);
            // 读取温湿度数据
            retval = AHT20_GetMeasureResult(&temperature, &humidity);
            printf("temp: %.2f,  humi: %.2f\n", temperature, humidity);

            // OLED显示数据
            OledShowString(0, 0, "Env data:", 1);
            // 温度显示
            char temp_str[20];
            snprintf(temp_str, sizeof(temp_str), "Temp: %.2f", temperature);
            OledShowString(0, 1, temp_str, 1);
            // 湿度显示
            char humi_str[20];
            snprintf(humi_str, sizeof(humi_str), "Humi: %.2f", humidity);
            OledShowString(0, 2, humi_str, 1);
            // 燃气数值显示
            char gas_str[20];
            snprintf(gas_str, sizeof(gas_str), "Gas: %d", data);
            OledShowString(0, 3, gas_str, 1);
        }
        sleep(1);
    }
}

// 创建线程
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
APP_FEATURE_INIT(AhtDemoTaskEntry);