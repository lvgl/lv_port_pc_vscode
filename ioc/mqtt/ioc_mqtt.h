#ifndef __IOC_MQTT_H__
#define __IOC_MQTT_H__

#define CERTSPATH      "/etc/ssl/certs/"

// 订阅的主题 Topics
typedef enum
{
	SUBSCRIBE_SYSCONFIG = 0, // SysConfig.json
	SUBSCRIBE_UI_GLOBAL,     // UI.json global部分
	SUBSCRIBE_UI_CONNS,      // UI.json conns部分
	SUBSCRIBE_TRIGGER,       // Trigger
	SUBSCRIBE_TestRGB,       // 测试主题

	SUBSCRIBE_NUM
}Subscribe_List_t;

typedef enum
{
	PUBLISH_UI = 0,    // UI.json
	PUBLISH_TestResp,    // UI.json
	PUBLISH_NUM
}Publish_List_t;

#define MQTT_SUBSCRIBE_MAX_NUM  SUBSCRIBE_NUM

// MQTT连接状态
typedef enum
{
	MQTT_CONNECT_NOT = 0,	// 未连接
	MQTT_CONNECTING,		// 正在连接中
	MQTT_CONNECTED			// 已连接
}Mqtt_Connetc_Status;

// 订阅
typedef struct
{
	char *          pSubscribeName; // 订阅主题名称
}Mqtt_Subscribe_t;

typedef struct
{
    Mqtt_Subscribe_t    List[MQTT_SUBSCRIBE_MAX_NUM]; // 订阅列表
    int                 subId;
}Subscribe_t;

Subscribe_t s_subscribe;


int ioc_mqtt_init();
void ioc_mqtt_step();
int ioc_mqtt_publish(uint8_t connId, Publish_List_t list, char * pData);

#endif