#include "../pages_mngr.h"

#define UI_NAME_HELP1_CLOSE_BTN         ".help1_close_button"
#define UI_NAME_HELP1_HELP_LIST         ".help1_list"
#define UI_NAME_HELP1_LABEL             ".help1_title"
#define UI_NAME_HELP1_CLOSE_LABEL       ".help1_close_button.btn_label"
#define UI_NAME_HELP1_INFO_RF           ".help1_status_info.info_RF_img"
#define UI_NAME_HELP1_INFO_4G           ".help1_status_info.info_4G_img"
#define UI_NAME_HELP1_INFO_SEVER        ".help1_status_info.info_sever_img"
#define UI_NAME_HELP1_INFO_NET          ".help1_status_info.info_net_img"
#define UI_NAME_HELP1_INFO_WIFI         ".help1_status_info.info_wifi_img"
#define UI_NAME_HELP1_INFO_METER        ".help1_status_info.info_meter_img"
#define UI_NAME_HELP1_INFO_IP           ".help1_status_info.IP_lable"


#define UI_NAME_HELP2_CLOSE_BTN         ".help2_close_button"
#define UI_NAME_HELP2_BACK_BTN          ".help2_back_button"
#define UI_NAME_HELP2_TITLE             ".help2_title"
#define UI_NAME_HELP2_CONTENT           ".help2_content"
#define UI_NAME_HELP2_CLOSE_LABEL       ".help2_close_button.btn_label"
#define UI_NAME_HELP2_BACK_LABEL        ".help2_back_button.btn_label"

typedef struct
{
    lv_obj_t * help1;                                       // 第一层帮助页面
    lv_obj_t * help1_label;                                 // 第一层帮助页面的帮助文本
    lv_obj_t * help1_close_label;                           // 第一层帮助页面的按钮文本
    lv_obj_t * help1_info_rf;                               // 第一层帮助页面的RF信息
    lv_obj_t * help1_info_4g;                               // 第一层帮助页面的RF信息
    lv_obj_t * help1_info_sever;                            // 第一层帮助页面的server信息
    lv_obj_t * help1_info_net;                              // 第一层帮助页面的net信息
    lv_obj_t * help1_info_wifi;                             // 第一层帮助页面的wifi信息
    lv_obj_t * help1_info_meter;                            // 第一层帮助页面的meter信息
    lv_obj_t * help1_info_IP;                               // 第一层帮助页面的IP信息

    lv_obj_t * help2;                                       // 第二层帮助页面

    lv_obj_t * close_btn_help1;                             // 第一层页面的关闭按钮, 关闭帮助页面
    lv_obj_t * help_list;                                   // 第一层页面的帮助列表, 点击后打开第二层帮助页面

    lv_obj_t * close_btn_help2;                             // 第二层页面的关闭按钮, 直接关闭两层帮助页面
    lv_obj_t * back_btn;                                    // 第二层页面的返回按钮, 返回第一层页面
    lv_obj_t * title;                                       // 第二层页面的标题
    lv_obj_t * content;                                     // 第二层页面的内容
    lv_obj_t * help2_close_label;                           // 第二层帮助页面的关闭按钮文本
    lv_obj_t * help2_back_label;                            // 第二层帮助页面的返回按钮文本

    cJSON * help_json;                                      // 帮助的json数据, 从文件中读取
} ui_help_t;

static ui_help_t s_ui_help = {0};


static void update_img_if_changed(lv_obj_t **img_obj, int *last_state, int new_state, const char *on, const char *err, const char *off)
{
    if (new_state != *last_state)
    {
        const char *src = NULL;

        // 三态判断（1=on, 2=err, 其他=off）
        if (new_state == 1)
            src = on;
        else if (new_state == 2)
            src = err;
        else
            src = off;

        lv_img_set_src(*img_obj, src);
        *last_state = new_state;
    }
}

static void update_img_bool_if_changed(lv_obj_t **img_obj, bool *last_state, bool new_state, const char *on, const char *err)
{
    if (new_state != *last_state)
    {
        const char *src = new_state ? err : on;  // true = 错误
        // LV_LOG_WARN("src[%s]", src);
        lv_img_set_src(*img_obj, src);
        *last_state = new_state;
    }
}

static void ui_page_pop_help_status_update()
{
    // LV_LOG_WARN("2222222==============================");

    // LV_LOG_WARN("m4gStatus[%d] ethStatus[%d] wifiStatus[%d] meterFault[%d] rfFault[%d] online[%d]", g_ui_json.drvPointInfo.m4gStatus,
    //           g_ui_json.drvPointInfo.ethStatus, g_ui_json.drvPointInfo.wifiStatus, g_ui_json.drvPointInfo.meterFault,
    //           g_ui_json.drvPointInfo.rfFault, g_ui_json.drvPointInfo.online);


    static int last_4g = -1;
    static int last_eth = -1;
    static int last_wifi = -1;
    static bool last_meterFault = true;
    static bool last_rfFault = true;
    static bool last_online = false;

    // 4G 状态 (1=on, 2=err, else=off)
    update_img_if_changed(&s_ui_help.help1_info_4g, &last_4g,
                          g_ui_json.drvPointInfo.m4gStatus,
                          "A:./assets/images/4g_on.png",
                          "A:./assets/images/4g_err.png",
                          "A:./assets/images/4g_off.png");

    // 网线状态
    update_img_if_changed(&s_ui_help.help1_info_net, &last_eth,
                          g_ui_json.drvPointInfo.ethStatus,
                          "A:./assets/images/net_on.png",
                          "A:./assets/images/net_err.png",
                          "A:./assets/images/net_off.png");

    // Wi-Fi 状态
    update_img_if_changed(&s_ui_help.help1_info_wifi, &last_wifi,
                          g_ui_json.drvPointInfo.wifiStatus,
                          "A:./assets/images/wifi_on.png",
                          "A:./assets/images/wifi_err.png",
                          "A:./assets/images/wifi_off.png");

    // 电表故障（true=err, false=on）
    update_img_bool_if_changed(&s_ui_help.help1_info_meter, &last_meterFault,
                               g_ui_json.drvPointInfo.meterFault,
                               "A:./assets/images/meter_on.png",
                               "A:./assets/images/meter_err.png");

    // RF 故障（true=err, false=on）
    update_img_bool_if_changed(&s_ui_help.help1_info_rf, &last_rfFault,
                               g_ui_json.drvPointInfo.rfFault,
                               "A:./assets/images/rf_on.png",
                               "A:./assets/images/rf_err.png");

    // 服务器在线状态（true=on, false=err）
    update_img_bool_if_changed(&s_ui_help.help1_info_sever, &last_online,
                               g_ui_json.drvPointInfo.online,
                               "A:./assets/images/sever_err.png",
                               "A:./assets/images/sever_on.png");
}

static void ui_page_pop_help_updatejson(struct ioc_obj_user_data_s *user_data)
{
    // LV_LOG_WARN("11111==============================");
    ui_page_pop_help_status_update();
    if (s_ui_help.help1_info_IP != NULL)
    {
        char buf[128] = {0};
        if (g_ui_json.drvPointInfo.httpAddr[0] != '\0')
        {
            snprintf(buf, sizeof(buf), "IP: %s", g_ui_json.drvPointInfo.httpAddr);
        }
        ioc_label_set_text(s_ui_help.help1_info_IP, buf);
    }
}


// 显示帮助页面
void ui_popup_show_help(void)
{
    if (s_ui_help.help_json == NULL)
    {
        s_ui_help.help_json = JSON_FromFile(HELP_PATCH_FILE);
        if (s_ui_help.help_json == NULL)
        {
            s_ui_help.help_json = JSON_FromFile(HELP_JSON_FILE);
        }
    }

    ui_popup_show_mask(true);   // 显示遮罩层

    if (s_ui_help.help1 == NULL)
    {
        s_ui_help.help1 = ioc_layout_mngr_load_layout("help1", lv_layer_top());

        // 第一次刷新一下状态
        ui_page_pop_help_status_update();

        // 设置回调，用于更新rf,meter等状态
        if (s_ui_help.help1 != NULL)
        {
            // LV_LOG_WARN("000=====================================");
            ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(s_ui_help.help1);
            user_data->ui_json_update_cb = ui_page_pop_help_updatejson;
        }
    }

    if (s_ui_help.help1 != NULL)
    {
        lv_obj_clear_flag(s_ui_help.help1, LV_OBJ_FLAG_HIDDEN);
    }

    // 文本随语言的改变而改变
    ioc_label_set_text(s_ui_help.help1_label, "Help");
    ioc_label_set_text(s_ui_help.help1_close_label, "Close");
    ioc_label_set_text(s_ui_help.help2_close_label, "Close");
    ioc_label_set_text(s_ui_help.help2_back_label, "Back");
}

static void ui_help1_close_btn_event_cb(lv_event_t * e)
{
    if (s_ui_help.help2 != NULL)
    {
        lv_obj_add_flag(s_ui_help.help2, LV_OBJ_FLAG_HIDDEN);
    }
    lv_obj_add_flag(s_ui_help.help1, LV_OBJ_FLAG_HIDDEN);
    ui_popup_show_mask(false);   // 关闭遮罩层
}

static void ui_help2_back_btn_event_cb(lv_event_t * e)
{
    if (s_ui_help.help2 != NULL)
    {
        lv_obj_add_flag(s_ui_help.help2, LV_OBJ_FLAG_HIDDEN);
    }
}

static void ui_help1_show_help2_btn_event_cb(lv_event_t * e)
{
    if (s_ui_help.help2 == NULL)
    {
        s_ui_help.help2 = ioc_layout_mngr_load_layout("help2", lv_layer_top());
    }

    if (s_ui_help.help2 != NULL)
    {
        cJSON * item = (cJSON *)lv_event_get_user_data(e);
        cJSON * title = cJSON_GetObjectItem(item, "title");
        cJSON * content = cJSON_GetObjectItem(item, "content");
        ioc_label_set_text(s_ui_help.title, title->valuestring);
        ioc_label_set_text(s_ui_help.content, content->valuestring);

        lv_obj_clear_flag(s_ui_help.help2, LV_OBJ_FLAG_HIDDEN);
    }
}

void ui_after_create_help1_close_btn(const char * name, lv_obj_t * obj)
{
    s_ui_help.close_btn_help1 = obj;
    lv_obj_add_event_cb(obj, ui_help1_close_btn_event_cb, LV_EVENT_CLICKED, NULL);
}

void ui_after_create_help1_help_list(const char * name, lv_obj_t * obj)
{
    s_ui_help.help_list = obj;
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    if ((s_ui_help.help_json != NULL) && cJSON_IsArray(s_ui_help.help_json))
    {
        int size = cJSON_GetArraySize(s_ui_help.help_json);
        for (int i = 0; i < size; i++)
        {
            cJSON * item = cJSON_GetArrayItem(s_ui_help.help_json, i);
            if (item == NULL)
            {
                continue;
            }

            cJSON * title = cJSON_GetObjectItem(item, "title");
            cJSON * content = cJSON_GetObjectItem(item, "content");
            if ((title != NULL) && (content != NULL))
            {
                lv_obj_t * btn = lv_list_add_btn(obj, NULL, title->valuestring);
                lv_obj_add_event_cb(btn, ui_help1_show_help2_btn_event_cb, LV_EVENT_CLICKED, item);
            }
        }
    }
}

void ui_after_create_help1_label(const char * name, lv_obj_t * obj)
{
    s_ui_help.help1_label = obj;
}

void ui_after_create_help1_close_label(const char * name, lv_obj_t * obj)
{
    s_ui_help.help1_close_label = obj;
}

void ui_after_create_help2_close_label(const char * name, lv_obj_t * obj)
{
    s_ui_help.help2_close_label = obj;
}

void ui_after_create_help2_back_label(const char * name, lv_obj_t * obj)
{
    s_ui_help.help2_back_label = obj;
}

void ui_after_create_help1_info_rf(const char * name, lv_obj_t * obj)
{
    s_ui_help.help1_info_rf = obj;
}

void ui_after_create_help1_info_4g(const char * name, lv_obj_t * obj)
{
    s_ui_help.help1_info_4g = obj;
}

void ui_after_create_help1_info_sever(const char * name, lv_obj_t * obj)
{
    s_ui_help.help1_info_sever = obj;
}

void ui_after_create_help1_info_net(const char * name, lv_obj_t * obj)
{
    s_ui_help.help1_info_net = obj;
}

void ui_after_create_help1_info_wifi(const char * name, lv_obj_t * obj)
{
    s_ui_help.help1_info_wifi = obj;
}

void ui_after_create_help1_info_meter(const char * name, lv_obj_t * obj)
{
    s_ui_help.help1_info_meter = obj;
}

#if 0
static bool get_default_iface(char *iface, size_t len)
{
    FILE *fp;
    char buf[128] = {0};
    char *p;

    fp = popen("ip route | grep default | head -n 1 | awk -F'dev' '{print $2}'", "r");
    if (fp == NULL)
    {
        return false;
    }

    if (fgets(buf, sizeof(buf), fp) == NULL)
    {
        pclose(fp);
        return false;
    }

    pclose(fp);

    // 去掉换行
    if ((p = strchr(buf, '\n')) != NULL)
    {
        *p = '\0';
    }

    // 跳过前导空格
    p = buf;
    while (*p == ' ')
    {
        p++;
    }

    // 截断到第一个空格（防止后面还有参数）
    char *space = strchr(p, ' ');
    if (space)
    {
        *space = '\0';
    }

    if (*p == '\0')
    {
        return false;
    }

    strncpy(iface, p, len - 1);
    iface[len - 1] = '\0';

    return true;
}
#endif

#if 0
void ui_after_create_help1_info_ip(const char *name, lv_obj_t *obj)
{
    s_ui_help.help1_info_IP = obj;

    char ip_addr[INET_ADDRSTRLEN] = {0};
    char buf[64] = {0};
    char iface[IFNAMSIZ] = {0};

    int fd = -1;
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));

    // 1. 获取默认出口网卡
    if (!get_default_iface(iface, sizeof(iface)))
    {
        LV_LOG_WARN("Failed to get default network interface");
        ioc_label_set_text(s_ui_help.help1_info_IP, "");
        return;
    }

    // 2. 创建 socket
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        LV_LOG_WARN("Failed to create socket");
        ioc_label_set_text(s_ui_help.help1_info_IP, "");
        return;
    }

    // 3. 获取该网卡 IP
    strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';

    if (ioctl(fd, SIOCGIFADDR, &ifr) == 0)
    {
        struct sockaddr_in *ipaddr = (struct sockaddr_in *)&ifr.ifr_addr;

        if (inet_ntop(AF_INET,
                      &ipaddr->sin_addr,
                      ip_addr,
                      sizeof(ip_addr)) != NULL)
        {
            snprintf(buf, sizeof(buf), "IP: %s", ip_addr);
        }
    }

    close(fd);

    ioc_label_set_text(s_ui_help.help1_info_IP, buf);
}
#endif

void ui_after_create_help1_info_ip(const char *name, lv_obj_t *obj)
{
    char buf[128] = {0};

    s_ui_help.help1_info_IP = obj;

    if (g_ui_json.drvPointInfo.httpAddr[0] != '\0')
    {
        snprintf(buf, sizeof(buf), "IP: %s", g_ui_json.drvPointInfo.httpAddr);
    }

    ioc_label_set_text(s_ui_help.help1_info_IP, buf);
}


void ui_after_create_help1(const char * name, lv_obj_t * obj)
{
    const char * help_name = name + strlen(UI_NAME_HELP1);
    if (strcmp(help_name, UI_NAME_HELP1_CLOSE_BTN) == 0)
    {
        ui_after_create_help1_close_btn(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP1_HELP_LIST) == 0)
    {
        ui_after_create_help1_help_list(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP1_LABEL) == 0)
    {
        ui_after_create_help1_label(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP1_CLOSE_LABEL) == 0)
    {
        ui_after_create_help1_close_label(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP1_INFO_RF) == 0)
    {
        ui_after_create_help1_info_rf(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP1_INFO_4G) == 0)
    {
        ui_after_create_help1_info_4g(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP1_INFO_SEVER) == 0)
    {
        ui_after_create_help1_info_sever(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP1_INFO_NET) == 0)
    {
        ui_after_create_help1_info_net(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP1_INFO_WIFI) == 0)
    {
        ui_after_create_help1_info_wifi(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP1_INFO_METER) == 0)
    {
        ui_after_create_help1_info_meter(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP1_INFO_IP) == 0)
    {
        // LV_LOG_WARN("222=========================================");
        ui_after_create_help1_info_ip(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP2_CLOSE_LABEL) == 0)
    {
        ui_after_create_help2_close_label(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP2_BACK_LABEL) == 0)
    {
        ui_after_create_help2_back_label(name, obj);
        return;
    }
}

void ui_after_create_help2_close_btn(const char * name, lv_obj_t * obj)
{
    s_ui_help.close_btn_help2 = obj;
    lv_obj_add_event_cb(obj, ui_help1_close_btn_event_cb, LV_EVENT_CLICKED, NULL);
}

void ui_after_create_help2_back_btn(const char * name, lv_obj_t * obj)
{
    s_ui_help.back_btn = obj;
    lv_obj_add_event_cb(obj, ui_help2_back_btn_event_cb, LV_EVENT_CLICKED, NULL);
}

void ui_after_create_help2_title(const char * name, lv_obj_t * obj)
{
    s_ui_help.title = obj;
}

void ui_after_create_help2_content(const char * name, lv_obj_t * obj)
{
    s_ui_help.content = obj;
}

void ui_after_create_help2(const char * name, lv_obj_t * obj)
{
    const char * help_name = name + strlen(UI_NAME_HELP2);
    if (strcmp(help_name, UI_NAME_HELP2_CLOSE_BTN) == 0)
    {
        ui_after_create_help2_close_btn(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP2_BACK_BTN) == 0)
    {
        ui_after_create_help2_back_btn(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP2_TITLE) == 0)
    {
        ui_after_create_help2_title(name, obj);
        return;
    }
    else if (strcmp(help_name, UI_NAME_HELP2_CONTENT) == 0)
    {
        ui_after_create_help2_content(name, obj);
        return;
    }
}
