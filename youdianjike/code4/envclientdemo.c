#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "hi_wifi_api.h"
#include "lwip/ip_addr.h"
#include "lwip/netifapi.h"
#include "lwip/sockets.h"

// 传感器头文件
#include "wifiiot_i2c.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_errno.h"
#include "aht20.h"
#include "wifiiot_adc.h"
#include "oled_ssd1306.h"

static char response[128] = ""; // 接收服务器回复

// 宏定义
#define AHT20_BAUDRATE 400 * 1000
#define AHT20_I2C_IDX WIFI_IOT_I2C_IDX_0
#define GAS_SENSOR_CHAN_NAME WIFI_IOT_ADC_CHANNEL_5

// 全局变量
uint32_t reval = 0;
float shiduval = 0.0f;
float wenduval = 0.0f;
unsigned short data = 0;
char line[32] = {0};

float shidutmp = 0.0f;
float wendutmp = 0.0f;
short qiweitmp = 0;
char strshidu[20] = {0}, strwendu[20] = {0}, strqiwei[20] = {0};
char linstr[80] = {0}; // 最终上传的字符串

// 设备初始化
void init(void)
{
    GpioInit();
    OledInit();
    OledFillScreen(0);
    I2cInit(AHT20_I2C_IDX, AHT20_BAUDRATE);

    // 配置I2C引脚
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_13, WIFI_IOT_IO_FUNC_GPIO_13_I2C0_SDA);
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_14, WIFI_IOT_IO_FUNC_GPIO_14_I2C0_SCL);

    // AHT20校准
    while (WIFI_IOT_SUCCESS != AHT20_Calibrate())
    {
        printf("AHT20 SENSOR init fail!!!\r\n");
        usleep(2000);
    }
}

// 连接TCP服务器，循环上传数据
void conent_tcp_server(const char *host, unsigned short port)
{
    init(); // 初始化设备
    //while (1) 
    //{
        // 读取传感器数据
        if (AHT20_StartMeasure() != WIFI_IOT_SUCCESS)
        {
            printf("AHT20 CELIANG fail!!\r\n");
        }
        if (AHT20_GetMeasureResult(&wenduval, &shiduval) != WIFI_IOT_SUCCESS)
        {
            printf("jieshou fail!!!");
        }
        AdcRead(GAS_SENSOR_CHAN_NAME, &data, WIFI_IOT_ADC_EQU_MODEL_4, WIFI_IOT_ADC_CUR_BAIS_DEFAULT, 0);

        // OLED显示
        OledShowString(0, 0, "Sensor values:", 1);
        snprintf(line, sizeof(line), "wen du:%.2f", wenduval);
        OledShowString(0, 1, line, 1);
        snprintf(line, sizeof(line), "shi du:%.2f", shiduval);
        OledShowString(0, 2, line, 1);
        snprintf(line, sizeof(line), "yan wu:%d", data);
        OledShowString(0, 3, line, 1);

        // 数据格式化，拼接为字符串
        wendutmp = wenduval;
        sprintf(strwendu, "%.2f", wendutmp);
        shidutmp = shiduval;
        sprintf(strshidu, "%.2f", shidutmp);
        qiweitmp = data;
        sprintf(strqiwei, "%d", qiweitmp);
        sprintf(linstr, "%s,%s,%s", strwendu, strshidu, strqiwei);
        printf("linstr:%s\r\n", linstr);

        sleep(2); // 每2秒上传一次

        // 网络连接与数据上传
        ssize_t retval = 0;
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in serverAddr = {0};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        if (inet_pton(AF_INET, host, &serverAddr.sin_addr) <= 0)
        {
            printf("inet_pton failed!\r\n");
            goto do_cleanup;
        }

        if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            printf("connect failed!\r\n");
            goto do_cleanup;
        }
        printf("connect to server %s success!\r\n", host);

        // 发送环境数据
        retval = send(sockfd, linstr, sizeof(linstr), 0);
        if (retval < 0)
        {
            printf("send request failed!\r\n");
            goto do_cleanup;
        }
        printf("send request{%s} %ld to server done!\r\n", linstr, retval);

        // 接收服务器回复
        retval = recv(sockfd, response, sizeof(response), 0);
        if (retval <= 0)
        {
            printf("recv response failed or done, %ld!\r\n", retval);
            goto do_cleanup;
        }
        response[retval] = '\0';
        printf("recv response{%s} %ld from server done!\r\n", response, retval);

    do_cleanup:
        printf("do_cleanup...\r\n");
        closesocket(sockfd);
    //}
}