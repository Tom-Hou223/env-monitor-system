#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "oled_ssd1306.h"

// 初始化设备
void init(void)
{
    GpioInit();
    OledInit();//初始化OLED屏幕
}

// OLED显示任务函数
static void OledTask(void *arg){
    (void)arg;
    init();//调用初始化方法
    OledFillScreen(0x00);//清空屏幕
    OledShowString(0, 0, "Hello, HarmonyOS", 1);//0列0行显示字符串
    sleep(1);
    for (int i = 0; i < 3; i++) {
        OledFillScreen(0x00);//清空屏幕
        for (int y = 0; y < 8; y++){
            static const char text[] = "ABCDEFGHIJKLMNOP";
            OledShowString(0, y, text, 1);
        }
        sleep(1);
    }
}

// 创建线程函数并初始化
static void OledTaskEntry(void)
{
    osThreadAttr_t attr;
    attr.name = "OledTask";
    attr.attr_bits = 0;
    attr.cb_mem = NULL;
    attr.cb_size = 0;
    attr.stack_mem = NULL;
    attr.stack_size = 2048;
    attr.priority = osPriorityNormal;

    if (osThreadNew(OledTask, NULL, &attr) == NULL)
    {
        printf("Failed to create OledTask!\n");
    }
}
// 应用初始化
APP_FEATURE_INIT(OledTaskEntry);