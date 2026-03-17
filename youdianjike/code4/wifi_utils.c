#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifi_device.h"
#include "lwip/netifapi.h"
#include "lwip/api_shell.h"
#include "lwip/ip4_addr.h"

void connect_wifi(void)
{
    WifiErrorCode errCode;
    WifiDeviceConfig apConfig = {};
    int netId = -1;

    // 你的 WiFi 信息
    strcpy(apConfig.ssid, "RayiPhone");
    strcpy(apConfig.preSharedKey, "88888888");
    apConfig.securityType = WIFI_SEC_TYPE_PSK;

    // 启动 WiFi
    errCode = EnableWifi();
    AddDeviceConfig(&apConfig, &netId);
    errCode = ConnectTo(netId);
    printf("ConnectTo: %d\r\n", errCode);

    // 先等 WiFi 物理层连接稳定（3秒）
    osDelay(300); // 3000ms

    struct netif *iface = netifapi_netif_find("wlan0");
    if (iface)
    {
        // 启动 DHCP
        err_t ret = netifapi_dhcp_start(iface);
        printf("dhcp start: %d\r\n", ret);

        // 【关键】轮询等待 DHCP 拿到有效 IP（最多等 10 秒）
        int wait_cnt = 0;
        const ip4_addr_t *ip_addr = netif_ip4_addr(iface);
        while (ip4_addr_isany(ip_addr) && wait_cnt < 20)
        {                // 每500ms查一次，最多20次=10秒
            osDelay(50); // 500ms
            ip_addr = netif_ip4_addr(iface);
            wait_cnt++;
            printf("waiting for dhcp... (%d)\r\n", wait_cnt);
        }

        // 打印最终拿到的 IP
        printf("=== WIFI Connected ===\r\n");
        printf("IP: %s\r\n", ip4addr_ntoa(ip_addr));
        printf("MASK: %s\r\n", ip4addr_ntoa(netif_ip4_netmask(iface)));
        printf("GW: %s\r\n", ip4addr_ntoa(netif_ip4_gw(iface)));
        printf("======================\r\n");

        // 如果还是 0.0.0.0，直接报错
        if (ip4_addr_isany(ip_addr))
        {
            printf("DHCP failed! Fallback to static IP.\r\n");
            // 这里可以加 fallback 静态 IP 逻辑（和你之前的静态代码一样）
        }
    }
}