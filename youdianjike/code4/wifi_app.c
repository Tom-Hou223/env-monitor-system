#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "hi_wifi_api.h"
#include "lwip/ip_addr.h"
#include "lwip/netifapi.h"
#include "lwip/sockets.h"

#define SSID "\xe8\xbd\xaf\xe4\xbb\xb6\xe5\xa4\x96\xe5\x8c\x85\xe5\xad\xa6\xe9\x99\xa2"
#define PASSWORD "wbxy@8888"

static struct netif *g_lwip_netif = NULL;

/**
* @brief  Set netif's ip, gatewayand netmask
* [url=home.php?mod=space&uid=3142012]@param[/url]  pst_lwip_netif
* @retval None
*/
void hi_sta_set_addr(struct netif *pst_lwip_netif)
{
    ip4_addr_t st_gw;
    ip4_addr_t st_ipaddr;
    ip4_addr_t st_netmask;
    if (pst_lwip_netif == NULL)
    {
        printf("hisi_reset_addr::Nullparam of netdev\r\n");
        return;
    }
    IP4_ADDR(&st_gw, 192, 168, 1, 1);
    IP4_ADDR(&st_ipaddr, 192, 168, 1,52);
    IP4_ADDR(&st_netmask, 255, 255, 255,0);
    netifapi_netif_set_addr(pst_lwip_netif,&st_ipaddr, &st_netmask, &st_gw);
}
/**
* @brief  Wifi connect
* @param  None
* @retval None
*/
int hi_wifi_start_connect(void)
{
    int ret;
    errno_t rc;
    hi_wifi_assoc_request assoc_req = {0};
    // Copy SSID to assoc_req
    rc = memcpy_s(assoc_req.ssid,HI_WIFI_MAX_SSID_LEN + 1, SSID, strlen(PASSWORD));
    if (rc != EOK)
    {
        printf("[Wifi Connnect]hi_wifi_sta_connect fail");
        printf("%s %d \r\n",__FILE__, __LINE__);
        return -1;
    }
    //Set encryption method
    assoc_req.auth = HI_WIFI_SECURITY_WPA2PSK;
    // Wifi password
    memcpy(assoc_req.key, PASSWORD,strlen(PASSWORD));
    ret = hi_wifi_sta_connect(&assoc_req);
    if (ret != HISI_OK)
    {
       printf("[WifiConnnect] hi_wifi_sta_connect fail");
        printf("%s %d \r\n",__FILE__, __LINE__);
        return -1;
    }
       return 0;
}
/**
* @brief  wifi task
* @param  None
* @retval None
*/
void wifi_task(void)
{
    int ret;
    char ifname[WIFI_IFNAME_MAX_SIZE + 1] ={0};
    int len = sizeof(ifname);
    //unsigned int  num = WIFI_SCAN_AP_LIMIT;
       //Step 1: Start STA mode, AT+STARTSTA
    ret = hi_wifi_sta_start(ifname, &len);
    if (ret != HISI_OK)
    {
        printf("[Wifi Connnect]hi_wifi_sta_start fail");
        printf("%s %d \r\n",__FILE__, __LINE__);
        return;
    }
    // Step 2: Connect to the specified AP:,AT+CONN="SSID", ,2,"PASSWORD"
    ret = hi_wifi_start_connect();
    if (ret != 0)
    {
        printf("[Wifi Connnect]hi_wifi_start_connect fail");
        printf("%s %d \r\n",__FILE__, __LINE__);
        return ;
    }
    // Step 3: DHCP requests the IP address ofwlan0 from the AP, AT+DHCP=wlan0,1  
    g_lwip_netif = netifapi_netif_find(ifname);
    if(NULL == g_lwip_netif)
    {
        printf("[Wifi Connnect]netifapi_netif_find fail");
        printf("%s %d \r\n",__FILE__, __LINE__);
        return;
    }     
    //DHCP automatically assigns IP
    if(ret !=netifapi_dhcp_start(g_lwip_netif))
    {
        printf("[Wifi Connnect]netifapi_dhcp_start fail");
        return;
    }     
    printf("[Wifi Connnect] Connect towifi successfully\n");
}
SYS_RUN(wifi_task);
