#include "ioc_app.h"
#include <stdio.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
#include <stdlib.h>
// #include <unistd.h>
#include <fcntl.h>
#include "mosquitto.h"

// 测试用
// #define MQTT_TEST

#if (COMPILER_FOR == COMPILE_FOR_CABINET_SSD202)

static struct mosquitto *s_mosq;
static Mqtt_Connetc_Status s_Connect_Satus;
static bool IsMsg = false;

static int last_connNum = 0;
static int last_connIdx[UI_MAX_CONNECTOR_NUM] = {-1, -1}; // 记录上一次的枪号，初始化为-1
static int last_autGroup = -1;   // 记录上一次的授权组

static bool TestMode = false;
// 定义最大主题长度
#define MAX_TOPIC_LENGTH 128

// 订阅的主题名称
const char * c_mqttSubscribeNames[SUBSCRIBE_NUM] =
{
    "/global-sysconfig",
    "/homeOcpp/UI/global-data",
    "/homeOcpp/UI/conn-%d",
    "/authGroup/display-%d/trigger",
    "/TestRGB"
};

// 发布的主题名称
const char * c_mqttPublishNames[PUBLISH_NUM] =
{
    "/homeOcpp/UI",
    "/TestRGB_RESP"
};

typedef struct
{
    char address[64];  // MQTT 代理地址
    int  port;         // 端口号
    char clientID[64]; // 客户端 ID
} MqttConfig;

MqttConfig s_mqttConfig;  // mqtt 配置address 和 port 从UDP 获取

#define BUFFER_SIZE     1024
#define QOS             1                    // 服务质量 (0, 1, 2)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 发布
int ioc_mqtt_publish(uint8_t connId, Publish_List_t list, char * pData)
{
    char pPublish[128] = {0};
	bool retain = true;

    if (s_Connect_Satus != MQTT_CONNECTED)
    {
        LV_LOG_WARN("MQTT not connected");
        return -1;
    }

    if(pData == NULL)
    {
        LV_LOG_WARN("pData is NULL");
        return -1;
    }

    snprintf(pPublish, sizeof(pPublish), "%s", c_mqttPublishNames[list]);

	return mosquitto_publish(s_mosq, NULL, pPublish, strlen(pData), pData, 2, retain);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool get_json_u8(cJSON *root, const char *fmt, int idx, int *out_val)
{
    char key[32] = {0};
    snprintf(key, sizeof(key), fmt, idx);
    cJSON *item = cJSON_GetObjectItem(root, key);
    if (item && cJSON_IsString(item))
    {
        *out_val = atoi(item->valuestring);
        return true;
    }
    return false;
}
static void TestRGB_subscribe(void)
{
    if (s_Connect_Satus == MQTT_CONNECTED)
    {
        int rc = mosquitto_subscribe(s_mosq, NULL, c_mqttSubscribeNames[SUBSCRIBE_TestRGB], QOS);
        // DEBUG_INFO("subscribe[%s]", new_topic);
        if (rc != MOSQ_ERR_SUCCESS)
        {
            LV_LOG_WARN("Error subscribing new conn topic: %s (%s)\n", c_mqttSubscribeNames[SUBSCRIBE_TestRGB], mosquitto_strerror(rc));
            mosquitto_disconnect(s_mosq);
        }
        else
        {
            LV_LOG_WARN("new_topic[%s]", c_mqttSubscribeNames[SUBSCRIBE_TestRGB]);
        }
    }
}
static void conns_subscribe(void)
{
    int new_connNum = UI_NUMOFCONNECTOR;

    // DEBUG_INFO("last_connNum[%d] new_connNum[%d]", last_connNum, new_connNum);

    // 先取消多余的订阅（如果枪数量减少了）
    for (int i = new_connNum; i < last_connNum; i++)
    {
        LV_LOG_WARN("i[%d] last_connIdx[%d]", i, last_connIdx[i]);
        if (last_connIdx[i] >= 0)
        {
            char old_topic[MAX_TOPIC_LENGTH] = {0};
            snprintf(old_topic, MAX_TOPIC_LENGTH, c_mqttSubscribeNames[SUBSCRIBE_UI_CONNS], last_connIdx[i]);

            if (s_Connect_Satus == MQTT_CONNECTED)
            {
                int rc = mosquitto_unsubscribe(s_mosq, NULL, old_topic);
                // DEBUG_INFO("unsubscribe[%s]", old_topic);
                if (rc != MOSQ_ERR_SUCCESS)
                {
                    LV_LOG_WARN("Error unsubscribing old conn topic: %s (%s)\n", old_topic, mosquitto_strerror(rc));
                }
            }
            last_connIdx[i] = -1;
        }
    }

    // 遍历新枪数量，检查是否需要重新订阅
    for (int i = 0; i < new_connNum && i < UI_MAX_CONNECTOR_NUM; i++)
    {
        int new_idx = g_ui_json.dispenserConfig.connIdx[i];

        LV_LOG_WARN("i[%d] new_idx[%d] last_connIdx[%d]", i, new_idx, last_connIdx[i]);

        if (new_idx != last_connIdx[i])  // 如果枪号变了
        {
            LV_LOG_WARN("i[%d] new_idx[%d] last_connIdx[%d]", i, new_idx, last_connIdx[i]);

            // 取消旧的订阅
            if (last_connIdx[i] >= 0)
            {
                char old_topic[MAX_TOPIC_LENGTH] = {0};
                snprintf(old_topic, MAX_TOPIC_LENGTH, c_mqttSubscribeNames[SUBSCRIBE_UI_CONNS], last_connIdx[i]);

                LV_LOG_WARN("===old_topic[%s]", old_topic);


                if (s_Connect_Satus == MQTT_CONNECTED)
                {
                    int rc = mosquitto_unsubscribe(s_mosq, NULL, old_topic);
                    if (rc != MOSQ_ERR_SUCCESS)
                    {
                        LV_LOG_WARN("Error unsubscribing old conn topic: %s (%s)\n", old_topic, mosquitto_strerror(rc));
                    }
                }
            }

            // 订阅新的
            char new_topic[MAX_TOPIC_LENGTH] = {0};
            snprintf(new_topic, MAX_TOPIC_LENGTH, c_mqttSubscribeNames[SUBSCRIBE_UI_CONNS], new_idx);

            // LV_LOG_WARN("new_topic[%s]", new_topic);

            if (s_Connect_Satus == MQTT_CONNECTED)
            {
                int rc = mosquitto_subscribe(s_mosq, NULL, new_topic, QOS);
                // DEBUG_INFO("subscribe[%s]", new_topic);
                if (rc != MOSQ_ERR_SUCCESS)
                {
                    LV_LOG_WARN("Error subscribing new conn topic: %s (%s)\n", new_topic, mosquitto_strerror(rc));
                    mosquitto_disconnect(s_mosq);
                }
                else
                {
                    LV_LOG_WARN("new_topic[%s]", new_topic);
                }
            }

            last_connIdx[i] = new_idx;
        }
    }

    // 更新记录的枪数量
    last_connNum = new_connNum;
}


static void authGroup_subscribe()
{
    // 如果授权组变化了，重新订阅
    if (g_ui_json.dispenserConfig.autGroup != last_autGroup)
    {
        // 如果已经有旧的订阅，先取消
        if (last_autGroup >= 0)
        {
            char old_topic[MAX_TOPIC_LENGTH] = {0};
            snprintf(old_topic, MAX_TOPIC_LENGTH, c_mqttSubscribeNames[SUBSCRIBE_TRIGGER], last_autGroup);

            if (s_Connect_Satus == MQTT_CONNECTED)
            {
                int rc = mosquitto_unsubscribe(s_mosq, NULL, old_topic);
                if (rc != MOSQ_ERR_SUCCESS)
                {
                    LV_LOG_WARN("Error unsubscribing old topic: %s\n", mosquitto_strerror(rc));
                }
                else
                {
                    LV_LOG_WARN("Unsubscribed old topic: %s", old_topic);
                }
            }
        }

        // 拼接新的订阅主题
        char new_topic[MAX_TOPIC_LENGTH] = {0};
        snprintf(new_topic, MAX_TOPIC_LENGTH, c_mqttSubscribeNames[SUBSCRIBE_TRIGGER], g_ui_json.dispenserConfig.autGroup);

        if (s_Connect_Satus == MQTT_CONNECTED)
        {
            int rc = mosquitto_subscribe(s_mosq, NULL, new_topic, QOS);
            if (rc != MOSQ_ERR_SUCCESS)
            {
                LV_LOG_WARN("Error subscribing new topic: %s\n", mosquitto_strerror(rc));
                mosquitto_disconnect(s_mosq);
            }
            else
            {
                LV_LOG_WARN("Subscribed new topic: %s", new_topic);
            }
        }

        // 更新记录的授权组
        last_autGroup = g_ui_json.dispenserConfig.autGroup;
    }
}


static void handle_sysconfig_subscribe(const char *value)
{
    // LV_LOG_WARN("===%s", value);
    // LV_LOG_WARN("RECEIVE SYSCONFIG========");

    if (value == NULL || *value == '\0')
    {
        LV_LOG_WARN("handle_sysconfig_subscribe: empty input");
        return;
    }

    cJSON *root = cJSON_Parse(value);
    if (root == NULL)
    {
        LV_LOG_WARN("Failed to parse JSON");
        return;
    }

    cJSON * rfNum = cJSON_GetObjectItem(root, "rfNum");
    if (rfNum == NULL)
    {
        LV_LOG_WARN("No rfNum");
        cJSON_Delete(root);
        return;
    }

    cJSON * connectorNum = cJSON_GetObjectItem(root, "connectorNum");
    if (connectorNum == NULL)
    {
        LV_LOG_WARN("No connectorNum");
        cJSON_Delete(root);
        return;
    }

    int uiScreenNum = atoi(rfNum->valuestring);
    int uiConnNum = atoi(connectorNum->valuestring);

    // 屏幕处于哪个终端上
    uint8_t loc = g_ui_json.dispenserConfig.location;

    // 保存旧值，用于变化检测
    uint8_t oldConnNum = g_ui_json.dispenserConfig.connNum;
    uint8_t oldAutGroup = g_ui_json.dispenserConfig.autGroup;
    uint8_t oldConnIdx[UI_MAX_CONNECTOR_NUM];
    memcpy(oldConnIdx, g_ui_json.dispenserConfig.connIdx, sizeof(oldConnIdx));
    static bool first_call = true;

    g_ui_json.dispenserConfig.connNum = 0;

    for (uint8_t screenIdx = 0; screenIdx < uiScreenNum; screenIdx++)
    {
        int temp_loc = -1;
        get_json_u8(root, "rfDispenser%d", screenIdx, &temp_loc);
        //  先根据终端号找到屏幕idx
        if (temp_loc == loc)
        {
            g_ui_json.dispenserConfig.autGroup = screenIdx;
            int tmp_connIdx = -1;
            // 轮询哪根枪是属于这个屏幕
            for(int connIdx = 0; connIdx < uiConnNum; connIdx++)
            {
                get_json_u8(root, "connMap2Rf%d", connIdx, &tmp_connIdx);
                if(tmp_connIdx == screenIdx)
                {
                    // LV_LOG_WARN("=======tmp[%d]=====screenIdx[%d]", tmp, screenIdx);
                    if(g_ui_json.dispenserConfig.connNum < UI_MAX_CONNECTOR_NUM)
                    {
                        g_ui_json.dispenserConfig.connIdx[g_ui_json.dispenserConfig.connNum] = connIdx;
                        g_ui_json.dispenserConfig.connNum ++;
                    }
                    else
                    {
                        LV_LOG_WARN("Exceeded maximum number of display connectors");
                        break;
                    }
                }
            }
            break;  // 找到对应项后退出
        }
    }

    // 比较前后配置是否变化
    bool changed = false;
    if (g_ui_json.dispenserConfig.connNum != oldConnNum ||
        g_ui_json.dispenserConfig.autGroup != oldAutGroup ||
        memcmp(g_ui_json.dispenserConfig.connIdx, oldConnIdx, sizeof(oldConnIdx)) != 0)
    {
        changed = true;
    }

    if (!first_call && !changed)
    {
        LV_LOG_WARN("handle_sysconfig_subscribe: configuration unchanged, skipping");
        cJSON_Delete(root);
        return;
    }

    LV_LOG_WARN("======rfNum[%d]", uiScreenNum);
    LV_LOG_WARN("======================loc[%d]", loc);
    LV_LOG_WARN("g_ui_json.dispenserConfig.connNum[%d]", g_ui_json.dispenserConfig.connNum);
    LV_LOG_WARN("g_ui_json.dispenserConfig.autGroup[%d]", g_ui_json.dispenserConfig.autGroup);
    for (int i = 0; i < g_ui_json.dispenserConfig.connNum; i++)
    {
        LV_LOG_WARN("g_ui_json.dispenserConfig.connIdx[%d]", g_ui_json.dispenserConfig.connIdx[i]);
    }

    ioc_ui_json_set_con_num(g_ui_json.dispenserConfig.connNum);

    // 此处屏幕已经知道自己在哪个授权组上，订阅Triger相关主题
    authGroup_subscribe();

    // 根据枪数量和枪id订阅要显示枪的主题
    conns_subscribe();
    // 订阅测试主题
    TestRGB_subscribe();
    cJSON_Delete(root);
}
bool ioc_ui_mqtt_trigger_parse(cJSON *trigger)
{
    const char *trigger_str = cJSON_GetStringValue(cJSON_GetObjectItem(trigger, "Trigger"));
    // 以下字段不一定有，需要根据Trigger类型进行判断
    cJSON *Msg = cJSON_GetObjectItem(trigger, "Msg");
    if (strcmp(trigger_str, "Test") == 0)
    {
        if (Msg != NULL)
        {
            uint8_t color_idx = (uint8_t)Msg->valueint;
            ui_trigger_panel_show(color_idx);
        }
        return true;
    }
    else if (strcmp(trigger_str, "Cancel") == 0)
    {
        ui_trigger_panel_hide();
        return false;
    }
    return false;
}
static void handle_TestRGB_subscribe(const char *value)
{
    // LV_LOG_WARN("RECEIVE UI JSON========");
    if (!value)
    {
        LV_LOG_WARN("value is NULL");
        return;
    }

    if(strlen(value) > UI_JSON_BUFFER_SIZE)
    {
        LV_LOG_WARN("value too long");
    }

    cJSON *root = cJSON_Parse(value);
    if (cJSON_GetObjectItem(root, "Trigger"))
    {
        TestMode = ioc_ui_mqtt_trigger_parse(root);
    }
    //
    if (TestMode)
    {
        mosquitto_publish(s_mosq, NULL, c_mqttPublishNames[1], strlen("OK"), "OK", 2, false);
    }


    uint64_t notify = 1;
    if (write(g_ui_json_eventfd, &notify, sizeof(notify)) < 0)
    {
        LV_LOG_WARN("write to g_ui_json_eventfd failed");
        return;
    }

    memset(g_ui_json_buffer, 0x00, UI_JSON_BUFFER_SIZE);
    strncpy(g_ui_json_buffer, value, UI_JSON_BUFFER_SIZE - 1);
    g_ui_json_buffer[UI_JSON_BUFFER_SIZE - 1] = '\0';
}

static void handle_trigger_subscribe(const char *value)
{
    // LV_LOG_WARN("RECEIVE UI JSON========");
    if (!value)
    {
        LV_LOG_WARN("value is NULL");
        return;
    }

    if(strlen(value) > UI_JSON_BUFFER_SIZE)
    {
        LV_LOG_WARN("value too long");
    }

    if (g_ui_json_eventfd <= 0)
    {
        LV_LOG_WARN("Invalid g_ui_json_eventfd: %d", g_ui_json_eventfd);
        return;
    }

    uint64_t notify = 1;
    if (write(g_ui_json_eventfd, &notify, sizeof(notify)) < 0)
    {
        LV_LOG_WARN("write to g_ui_json_eventfd failed");
        return;
    }

    memset(g_ui_json_buffer, 0x00, UI_JSON_BUFFER_SIZE);
    strncpy(g_ui_json_buffer, value, UI_JSON_BUFFER_SIZE - 1);
    g_ui_json_buffer[UI_JSON_BUFFER_SIZE - 1] = '\0';
}


#define UI_JSON_PART_SIZE     (2048 * 4) // 每个片段最大字节数
static char ui_json_global[UI_JSON_PART_SIZE];
static char ui_json_conns[UI_MAX_CONNECTOR_NUM][UI_JSON_PART_SIZE];

// 保存 global-data 或 conn-%d JSON 片段
static void store_ui_json(const char *topic, const char *payload)
{
    // DEBUG_WARN("store_ui_json topic [%s], payload [%s]", topic, payload);

    if (strcmp(topic, c_mqttSubscribeNames[SUBSCRIBE_UI_GLOBAL]) == 0)
    {
        strncpy(ui_json_global, payload, UI_JSON_PART_SIZE - 1);
        ui_json_global[UI_JSON_PART_SIZE - 1] = '\0';
    }
    else
    {
        int connIdx = -1;
        if (sscanf(topic, "/homeOcpp/UI/conn-%d", &connIdx) == 1)
        {
            for (int i = 0; i < UI_NUMOFCONNECTOR; i++)
            {
                if (g_ui_json.dispenserConfig.connIdx[i] == connIdx)
                {
                    strncpy(ui_json_conns[i], payload, UI_JSON_PART_SIZE - 1);
                    ui_json_conns[i][UI_JSON_PART_SIZE - 1] = '\0';
                    break;
                }
            }
        }
    }
}


// 组装最终 JSON: {"global":{...}, "conns":[...]}
static void assemble_ui_json(void)
{
    cJSON *root = cJSON_CreateObject();

    // global 部分
    if (strlen(ui_json_global) > 0)
    {
        cJSON *global_json = cJSON_Parse(ui_json_global);
        if (global_json)
        {
            cJSON_AddItemToObject(root, "global", global_json);
        }
    }

    // LV_LOG_WARN("====================================");

    // conns 部分
    cJSON *conns_arr = cJSON_CreateArray();
    for (int i = 0; i < UI_NUMOFCONNECTOR; i++)
    {
        // DEBUG_WARN("ui_json_conns [%s]", ui_json_conns[i]);
        if (strlen(ui_json_conns[i]) > 0)
        {
            cJSON *conn_json = cJSON_Parse(ui_json_conns[i]);
            if (conn_json)
            {
                cJSON_AddItemToArray(conns_arr, conn_json);
            }
        }
    }
    cJSON_AddItemToObject(root, "conns", conns_arr);

    // 输出到全局 buffer
    char *out = cJSON_PrintUnformatted(root);
    if (out)
    {
        // DEBUG_WARN("out [%s]", out);

        memset(g_ui_json_buffer, 0, UI_JSON_BUFFER_SIZE);
        strncpy(g_ui_json_buffer, out, UI_JSON_BUFFER_SIZE - 1);

        uint64_t notify = 1;
        if (g_ui_json_eventfd >= 0)
        {
            write(g_ui_json_eventfd, &notify, sizeof(notify));
        }

        free(out);
    }

    cJSON_Delete(root);
}


// 处理 global-data / conn-%d 订阅
static void handle_ui_subscribe(const char *topic, const char *value)
{
    if (!topic || !value)
    {
        LV_LOG_WARN("topic or value is NULL");
        return;
    }

    if(strlen(value) > UI_JSON_BUFFER_SIZE)
    {
        LV_LOG_WARN("value too long");
    }

    if (g_ui_json_eventfd <= 0)
    {
        LV_LOG_WARN("Invalid g_ui_json_eventfd: %d", g_ui_json_eventfd);
        return;
    }

    store_ui_json(topic, value);
    assemble_ui_json();
}

static void mqtt_subscribe_handle(void *pData)
{
    if (pData == NULL) return;

    const struct mosquitto_message *msg = (const struct mosquitto_message *)pData;

    if (msg->topic == NULL || msg->payload == NULL)
    {
        LV_LOG_WARN("Invalid MQTT message received");
        return;
    }

    // LV_LOG_WARN("========msg->topic[%s]", msg->topic);

    if(strcmp(msg->topic, c_mqttSubscribeNames[SUBSCRIBE_SYSCONFIG]) == 0)
    {
        // 处理 /global-sysconfig 主题的消息
        // DEBUG_WARN("UI conn message received for global-sysconfig[%s]", (char *)msg->payload);
        handle_sysconfig_subscribe((char *)msg->payload);
    }
    else if(strcmp(msg->topic, c_mqttSubscribeNames[SUBSCRIBE_UI_GLOBAL]) == 0)
    {
        // 处理 /homeOcpp/UI/global-data 主题的消息
        // LV_LOG_WARN("UI conn message received for global-data");
        handle_ui_subscribe(msg->topic, (char *)msg->payload);

    }
    else if (strncmp(msg->topic, "/homeOcpp/UI/conn-", strlen("/homeOcpp/UI/conn-")) == 0)
    {
        // DEBUG_WARN("1111111=============================");

        int connIdx = -1;
        if (sscanf(msg->topic, "/homeOcpp/UI/conn-%d", &connIdx) == 1)
        {
            // LV_LOG_WARN("UI conn message received for conn=%d", connIdx);
            handle_ui_subscribe(msg->topic, (char *)msg->payload);
        }
        else
        {
            LV_LOG_WARN("Invalid conn topic format: %s", msg->topic);
        }
    }
    else if (strncmp(msg->topic, "/authGroup/display-", strlen("/authGroup/display-")) == 0)
    {
        int autGroupId = -1;
        if (sscanf(msg->topic, "/authGroup/display-%d/trigger", &autGroupId) == 1)
        {
            LV_LOG_INFO("Trigger received for authGroup=%d", autGroupId);
            handle_trigger_subscribe((char *)msg->payload);
        }
        else
        {
            LV_LOG_WARN("Invalid trigger topic format: %s", msg->topic);
        }
    }
    else if (strncmp(msg->topic, "/TestRGB", strlen("/TestRGB")) == 0)
    {
        handle_TestRGB_subscribe((char *)msg->payload);
    }
    else
    {
        LV_LOG_WARN("Unknown topic: %s", msg->topic);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Callback called when the client receives a CONNACK message from the broker. */
static void on_connect(struct mosquitto *mosq, void *obj, int reason_code)
{
	/* Print out the connection result. mosquitto_connack_string() produces an
	 * appropriate string for MQTT v3.x clients, the equivalent for MQTT v5.0
	 * clients is mosquitto_reason_string().
	 */

	LV_LOG_WARN("on_connect: %s\n", mosquitto_connack_string(reason_code));
	if (reason_code != 0)
    {
		/* If the connection fails for any reason, we don't want to keep on
		 * retrying in this example, so disconnect. Without this, the client
		 * will attempt to reconnect. */
		mosquitto_disconnect(mosq);
	}

	/* Making subscriptions in the on_connect() callback means that if the
	 * connection drops and is automatically resumed by the client, then the
	 * subscriptions will be recreated when the client reconnects. */
	for (uint8_t i = 0; i < MQTT_SUBSCRIBE_MAX_NUM; i++)
	{
		if (strlen(c_mqttSubscribeNames[i]) == 0)
		{
			continue; // 跳过未注册的订阅
		}

		int rc = mosquitto_subscribe(mosq, NULL, c_mqttSubscribeNames[i], QOS);
		if (rc != MOSQ_ERR_SUCCESS)
		{
			LV_LOG_WARN("Error subscribing: %s\n", mosquitto_strerror(rc));
			/* We might as well disconnect if we were unable to subscribe */
			mosquitto_disconnect(mosq);
		}
	}

	s_Connect_Satus = MQTT_CONNECTED;
}

static void on_disconnect(struct mosquitto *mosq, void *obj, int reason_code)
{
    LV_LOG_WARN("Lost link, Reason:%d", reason_code);

    // 断连时重置枪组信息
    last_connNum = 0;
    for (int i = 0; i < UI_MAX_CONNECTOR_NUM; i++)
    {
        last_connIdx[i] = -1;
    }

    last_autGroup = -1;

	s_Connect_Satus = MQTT_CONNECT_NOT;
}


/* Callback called when the broker sends a SUBACK in response to a SUBSCRIBE. */
static void on_subscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
	int i;
	bool have_subscription = false;

	/* In this example we only subscribe to a single topic at once, but a
	 * SUBSCRIBE can contain many topics at once, so this is one way to check
	 * them all. */
	for(i=0; i<qos_count; i++){
		LV_LOG_WARN("on_subscribe: %d:granted qos = %d\n", i, granted_qos[i]);
		if(granted_qos[i] <= 2){
			have_subscription = true;
		}
	}
	if(have_subscription == false){
		/* The broker rejected all of our subscriptions, we know we only sent
		 * the one SUBSCRIBE, so there is no point remaining connected. */
		fprintf(stderr, "Error: All subscriptions rejected.\n");
		mosquitto_disconnect(mosq);
	}
}

/* Callback called when the client receives a message. */
static void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	/* This blindly prints the payload, but the payload can be anything so take care. */
	// LV_LOG_WARN("MQTT message received: topic=%s, payload=%s", msg->topic, (char *)msg->payload);
	IsMsg = true;
	mqtt_subscribe_handle((void*)msg);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void mqtt_lineOff_check(void)
{
	if (s_Connect_Satus != MQTT_CONNECT_NOT)
	{
		return;
	}

	if (s_mqttConfig.port == 0)
	{
		return;
	}

    int rc;

#ifndef MQTT_TEST
    // 测试用，测试时隐藏
    // TLS 配置
    if (access("/tmp/mqtt/cert/client.mqtt.crt", F_OK) == 0)
    {
        rc = mosquitto_tls_set(s_mosq, NULL, "/etc/ssl/certs/", "/tmp/mqtt/cert/client.mqtt.crt", "/tmp/mqtt/cert/client.mqtt.key", NULL);
    }
    else
    {
        rc = mosquitto_tls_set(s_mosq, NULL, "/etc/ssl/certs/", NULL, NULL, NULL);
    }

	if (rc != MOSQ_ERR_SUCCESS)
    {
		// 文件不存在可能导致此错误
		LV_LOG_WARN("Error: %s\n", mosquitto_strerror(rc));
		s_Connect_Satus = MQTT_CONNECT_NOT;
		return;
	}
#endif
    LV_LOG_WARN("address[%s] port[%d]", s_mqttConfig.address, s_mqttConfig.port);

	rc = mosquitto_connect(s_mosq, s_mqttConfig.address, s_mqttConfig.port, 60);
	if (rc != MOSQ_ERR_SUCCESS)
    {
		LV_LOG_WARN("Error %s", mosquitto_strerror(rc));
		s_Connect_Satus = MQTT_CONNECT_NOT;
		return;
	}

    // 测试用，使用IP时需要
    // mosquitto_tls_insecure_set(s_mosq, true);

	s_Connect_Satus = MQTT_CONNECTING;

}


// 通过UDP获取mqttd的地址和端口
static bool mqtt_config_init()
{
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // 创建 UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        LV_LOG_WARN("socket error");
        return false;
    }

    // 设置非阻塞模式
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    // 绑定端口
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(DEV_AUTO_GROUP_PORT);

    // 允许多个进程绑定同一端口
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));

    int rtn = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (rtn < 0)
    {
        LV_LOG_WARN("socket error [%s]", strerror(errno));
        close(sockfd);
        return false;
    }

    // 记录开始时间
    time_t startTime = time(NULL);

    while (1)
    {
        ssize_t recvLen = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                                   (struct sockaddr*)&clientAddr, &addrLen);
        if (recvLen > 0)
        {
            buffer[recvLen] = '\0'; // 确保字符串终止

            // 解析 JSON 数据
            cJSON *root = cJSON_Parse(buffer);
            if (!root)
            {
                // writeLog(__LINE__, "1111111111", 0);
                close(sockfd);
                return false;
            }

            // 遍历 JSON 数组
            cJSON *item;
            cJSON_ArrayForEach(item, root)
            {
                cJSON *service = cJSON_GetObjectItem(item, "service");
                cJSON *desc = cJSON_GetObjectItem(item, "desc");
                cJSON *port = cJSON_GetObjectItem(item, "port");

                if (service && desc && port && cJSON_IsString(service) && cJSON_IsString(desc) && cJSON_IsNumber(port))
                {
                    if (strcmp(service->valuestring, "MQTT_BROKER") == 0)
                    {
                        strncpy(s_mqttConfig.address, desc->valuestring, sizeof(s_mqttConfig.address) - 1);
                        s_mqttConfig.address[sizeof(s_mqttConfig.address) - 1] = '\0';
                        s_mqttConfig.port = port->valueint;

                        LV_LOG_WARN("address[%s]  port[%d]", s_mqttConfig.address, s_mqttConfig.port);

                        // 其他参数填充默认值
                        // 生成随机的 clientID
                        memset(s_mqttConfig.clientID, 0x00, sizeof(s_mqttConfig.clientID));

                        float uptime;
                        float temp;
                        FILE *fp = fopen("/proc/uptime", "r");
                        if (fp != NULL) {
                            fscanf(fp, "%f %f", &uptime, &temp);
                            fclose(fp);
                            srand((unsigned int)(uptime * 1000));  // 转换为毫秒做种子
                        }

                        int randomNumber = rand(); // 生成 0 到 9999 之间的随机数
                        snprintf(s_mqttConfig.clientID,
                            sizeof(s_mqttConfig.clientID),
                            "dispenserScreen%d_%d",
                            randomNumber,
                            g_ui_json.dispenserConfig.location);

                        LV_LOG_INFO("clientID[%s]", s_mqttConfig.clientID);

                        cJSON_Delete(root);
                        close(sockfd);
                        return true; // 找到匹配项，成功返回
                    }
                }
            }
            cJSON_Delete(root);
        }
        usleep(100000); // 100ms 轮询
    }
}


int ioc_mqtt_init()
{
    LV_LOG_WARN("MQTT init start");

#ifndef MQTT_TEST
    // 测试用，测试时隐藏
    if(!mqtt_config_init())
    {
        return false;
    }
#endif

    LV_LOG_WARN("MQTT init success");

#ifdef MQTT_TEST
    // 测试用
    strcpy(s_mqttConfig.address, "192.168.2.36");
    s_mqttConfig.port = 1883;
    int randomNumber = rand(); // 生成 0 到 9999 之间的随机数
    snprintf(s_mqttConfig.clientID, sizeof(s_mqttConfig.clientID), "dispenserSreen%d", randomNumber);


    // 远程
    // strcpy(s_mqttConfig.address, "60.205.110.94");
    // s_mqttConfig.port = 2223;
    // int randomNumber = rand(); // 生成 0 到 9999 之间的随机数
    // snprintf(s_mqttConfig.clientID, sizeof(s_mqttConfig.clientID), "dispenserSreen%d", randomNumber);

#endif

    // 初始化 MQTT 库
    mosquitto_lib_init();

    // 创建 MQTT 客户端实例
    s_mosq = mosquitto_new(s_mqttConfig.clientID, true, NULL);
    if (s_mosq == NULL)
    {
        return -1;
    }

    // 设置已连接回调
    mosquitto_connect_callback_set(s_mosq, on_connect);
    // 设置发布回调
	mosquitto_subscribe_callback_set(s_mosq, on_subscribe);
    // 设置订阅回调
	mosquitto_message_callback_set(s_mosq, on_message);
    // 设置连接断开的回调
    mosquitto_disconnect_callback_set(s_mosq, on_disconnect);

    // // 订阅主题
    // if (s_Connect_Satus == MQTT_CONNECTED)
	// {
    //     for (int i = 0; i < SUBSCRIBE_NUM; i++)
    //     {
    //         int rc = mosquitto_subscribe(s_mosq, NULL, c_mqttSubscribeNames[i], QOS);
    //         if (rc != MOSQ_ERR_SUCCESS)
    //         {
    //             LV_LOG_WARN("Error subscribing: %s\n", mosquitto_strerror(rc));
    //             return -1;
    //         }
    //     }
	// }

    return 1;
}

void ioc_mqtt_step()
{
    mqtt_lineOff_check();

    do
    {
        IsMsg = false;
        mosquitto_loop(s_mosq, 0, 1);
    } while (IsMsg);
}

#endif
