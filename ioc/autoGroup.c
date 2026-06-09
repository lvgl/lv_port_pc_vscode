// #include "ioc_app.h"

// // extern int get_local_ip(char *ip);

// #define SERVICE_DISPLAY    "UI_DISPLAY"
// #define SERVICE_DESC       "LVGL"

// #define INVALID_SOCKET_FD (-1)

// const char* s_default_iface_name = "eth0";

// static int  s_fd_masterBroadCast = -1;
// static uint32_t  s_UdpSentTick = 0;
// static char s_devMasterIp[INET_ADDRSTRLEN] = {0};
// static char s_sendIP[INET_ADDRSTRLEN] = {0};
// static uint8_t s_mac_address[6] = {0};
// static char s_mac_string[18] = {0};

// struct ifreq ifr;

// uint32_t Charger_TimeNowMs(void)
// {
//     // return custom_tick_get();
// }


// static bool iface_get_ip()
// {
//     int tmp_sock = socket(AF_INET, SOCK_DGRAM, 0);
//     if (tmp_sock < 0)
//     {
//         LV_LOG_WARN("socket - create socket failed while get ip");
//         return false;
//     }

//     if(ioctl(tmp_sock, SIOCGIFADDR, &ifr) != 0)
//     {
//         LV_LOG_WARN("ioctl - get ip failed");
//         close(tmp_sock);
//         return false;
//     }

//     struct sockaddr_in *ipaddr = (struct sockaddr_in *)&ifr.ifr_addr;
//     inet_ntop(AF_INET, &ipaddr->sin_addr, s_devMasterIp, sizeof(s_devMasterIp));
//     close(tmp_sock);
//     return true;
// }


// // 创建一个UDP服务器，成功返回 fd，失败返回 -1
// static int MasterDevCreateUdp()
// {
//     int fd = INVALID_SOCKET_FD;

//     // 创建UdpSocket
//     fd = socket(AF_INET, SOCK_DGRAM, 0);
// 	if (fd < 0)
// 	{
// 		LV_LOG_WARN("socket err:%d, %s\n", errno, strerror(errno));
// 		return INVALID_SOCKET_FD;
// 	}

//     // 设置超时
//     struct timeval timeOutVal;
//     timeOutVal.tv_sec = 0;
//     timeOutVal.tv_usec = 10;
//     if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeOutVal, sizeof(timeOutVal)) < 0)
//     {
//         LV_LOG_WARN("SO_RCVTIMEO err:%d, %s\n", errno, strerror(errno));
//         close(fd);
//         return INVALID_SOCKET_FD;
//     }

//     // 支持广播
//     int opt = 1;
//     if(-1 == setsockopt(fd,SOL_SOCKET,SO_BROADCAST,&opt,sizeof(opt)))
//     {
//         LV_LOG_WARN("SO_BROADCAST err:%d, %s\n", errno, strerror(errno));
//         close(fd);
//         return INVALID_SOCKET_FD;
//     }

//     // 绑定接口
//     if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, s_default_iface_name, strlen(s_default_iface_name)) < 0)
//     {
//         LV_LOG_WARN("setsockopt - bind iface failed");
//         close(fd);
//         fd = -1;
//         return INVALID_SOCKET_FD;
//     }

//     // 获取MAC地址
//     strncpy(ifr.ifr_name, s_default_iface_name, IFNAMSIZ - 1);
//     if (ioctl(fd, SIOCGIFHWADDR, &ifr) == -1)
//     {
//         LV_LOG_WARN("ioctl failed");
//         close(fd);
//         return INVALID_SOCKET_FD;
//     }

//     memcpy(s_mac_address, ifr.ifr_hwaddr.sa_data, 6);

//     // 将MAC地址转成字符串格式
//     snprintf(s_mac_string, sizeof(s_mac_string),
//              "%02x:%02x:%02x:%02x:%02x:%02x",
//              s_mac_address[0], s_mac_address[1], s_mac_address[2],
//              s_mac_address[3], s_mac_address[4], s_mac_address[5]);

//     return fd;
// }

// // 广播发送dev master的信息
// static void autoGroup_send(int fd)
// {
//     cJSON *serviceArry = cJSON_CreateArray();
//     if(serviceArry == NULL) return;

//     cJSON *servicePos = cJSON_CreateObject();
//     cJSON_AddItemToArray(serviceArry, servicePos);

//     if (cJSON_AddStringToObject(servicePos, "service",   SERVICE_DISPLAY) == NULL    ) goto FAIL_JSON;
//     if (cJSON_AddStringToObject(servicePos, "ip",        s_devMasterIp) == NULL    ) goto FAIL_JSON;
//     if (cJSON_AddNumberToObject(servicePos, "port",      SENDUI_LCD3354_DST_PORT) == NULL) goto FAIL_JSON;
//     if (cJSON_AddStringToObject(servicePos, "mac",       s_mac_string) == NULL) goto FAIL_JSON;
//     if (cJSON_AddStringToObject(servicePos, "desc",      SERVICE_DESC) == NULL) goto FAIL_JSON;


//     char * pp = cJSON_PrintUnformatted(serviceArry);
//     if (NULL == pp) goto FAIL_JSON;

//     struct sockaddr_in  servaddr;

//     bzero(&servaddr, sizeof(servaddr));
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_addr.s_addr = inet_addr(s_sendIP);//htonl(INADDR_ANY);
//     servaddr.sin_port = htons(DEV_AUTO_GROUP_PORT);

//     // LV_LOG_WARN("Boardbase ip[%s] : Port[%d]", s_sendIP, (int)(DEV_AUTO_GROUP_PORT));

//     sendto(fd, pp, strlen(pp), 0, (struct sockaddr*)&servaddr, sizeof(struct sockaddr));

//     // DEBUG_BUFF(pp, strlen(pp));

//     cJSON_free(pp);

// FAIL_JSON:

//     cJSON_Delete(serviceArry);

// }


// void autoGroup_init(void)
// {
// #if (COMPILER_FOR == COMPILE_FOR_AC_SSD202)
//     s_UdpSentTick = Charger_TimeNowMs();

//     s_fd_masterBroadCast = MasterDevCreateUdp();

//     if (s_fd_masterBroadCast > 0)
//     {
//         LV_LOG_USER("Create Udp(send) fd = %d", s_fd_masterBroadCast);
//     }

//     memset(s_devMasterIp, 0x00, sizeof(s_devMasterIp));
//     snprintf(s_sendIP, sizeof(s_sendIP), "%d.%d.%d.%d", 255, 255, 255, 255);
// #endif
// }

// void autoGroup_step(void)
// {
// #if (COMPILER_FOR == COMPILE_FOR_AC_SSD202)

//     if (s_UdpSentTick + 1000 > Charger_TimeNowMs())
//     {
//         return;
//     }

//     s_UdpSentTick = Charger_TimeNowMs();

//     if (strlen(s_devMasterIp) == 0)
//     {
//         if (!iface_get_ip())
//         {
//             LV_LOG_WARN("get device ip failed");
//             return;
//         }

//         LV_LOG_WARN("Device IP [%s]", s_devMasterIp);
//     }

//     if (s_fd_masterBroadCast >= 0)
//     {
//         autoGroup_send(s_fd_masterBroadCast);
//     }

// #endif
// }
