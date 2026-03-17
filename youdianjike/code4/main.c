#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifi_device.h"
#include "lwip/netifapi.h"
#include "lwip/api_shell.h"
#include "wifi_utils.h"
#include "tcpclientdemo.h"

// 主任务函数
static void NetWorkTask(void *arg)
{
    (void)arg;
    connect_wifi(); // 连接WiFi
    printf("begin demo\n");
    unsigned short port = 5001;
    // 连接TCP服务器，IP替换为你电脑的局域网IP，端口5001
    conent_tcp_server("172.20.10.3", port);
}

// 创建线程
static void NetWorkDemo(void)
{
    osThreadAttr_t attr;

    attr.name = "NetWorkTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 10240;
    attr.priority = osPriorityNormal;

    if (osThreadNew(NetWorkTask, NULL, &attr) == NULL)
    {
        printf("[NetWorkDemo] Falied to create WifiConnectTask!\n");
    }
}
APP_FEATURE_INIT(NetWorkDemo);