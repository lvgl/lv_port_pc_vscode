#include "../pages_mngr.h"


#define UI_NAME_PRICE_RESERVED_LEFT_LABEL              "price_reserved_title"
#define UI_NAME_PRICE_RESERVED_PLUG                    "price_reserved_tip"
#define UI_NAME_PRICE_RESERVED_IDLE_TIME               "idle_time_value_reserved"

#define UI_NAME_PRICE_RESERVED_SPLIT_LINE              "price_reserved_divider"

#define UI_NAME_PRICE_RESERVED_CURRENT_IMG             "price_reserved_current.price_current_period_icon"
#define UI_NAME_PRICE_RESERVED_CURRENT_CIRCLE          "price_reserved_current.price_current_dot"
#define UI_NAME_PRICE_RESERVED_CURRENT_TIME_LABEL      "price_reserved_current.price_current_title"
#define UI_NAME_PRICE_RESERVED_CURRENT_PERIOD          "price_reserved_current.price_current_time"
#define UI_NAME_PRICE_RESERVED_CURRNET_PRICE_KWH       "price_reserved_current.price_current_rate.price_kwh"
#define UI_NAME_PRICE_RESERVED_CURRNET_PRICE_HR        "price_reserved_current.price_current_rate.price_hr"
#define UI_NAME_PRICE_RESERVED_CURRNET_PRICE_TIMES     "price_reserved_current.price_current_rate.price_times"
#define UI_NAME_PRICE_RESERVED_DEFAULT_PRICE_TEXT      "ocpp_price.label"


#define UI_RESERVED_DATA  g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.reserved
#define UI_RESERVED_SHOWE_PRICE g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].showPrice
#define UI_RESERVED_SHOWE_OCCPP_PRICE g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].isOcppPrice

typedef struct {

    lv_obj_t * page;
    lv_obj_t * idle_time;
    lv_obj_t * curr_period;
    lv_obj_t * curr_price_kwh;
    lv_obj_t * curr_price_hr;
    lv_obj_t * curr_price_times;
    lv_obj_t * default_price_text;
    lv_obj_t * curr_img;
    lv_obj_t * curr_circle;
    lv_obj_t * split_line;

    lv_obj_t * left_label;
    lv_obj_t * plug;
    lv_obj_t * current_time_label;

} ui_page_price_reserved_t;

static ui_page_price_reserved_t ui_page_price_reserved = {NULL};
static int s_reserved_show_price_flag = -1;      // -1:未收到UI.jison时的初始值  0：不显示价格 1：显示价格
static int s_reserved_show_OCPPprice_flag = -1;  // -1:未收到UI.jison时的初始值  0：显示正常价格 1：显示OCPP默认价格文本
static char s_reserved_price_text[256] = {0};
lv_timer_t * s_reserved_timer = NULL;

static void ui_page_price_reserved_time(lv_timer_t * temp)
{
    LV_UNUSED(temp);

    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        return;
    }
    int show_hour = 0;
    int show_min  = 0;
    int show_sec  = 0;
    if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE )
    {
        time_t current_time = time(NULL);
        time_t diftimer = difftime(UI_RESERVED_DATA.reverseTime, current_time);

        if (diftimer < 0) {
            diftimer = 0;
        }

        struct tm *tm = gmtime(&diftimer);

        if (current_time < UI_RESERVED_DATA.reverseTime) {
            show_hour = tm->tm_hour;
            show_min  = tm->tm_min;
            show_sec  = tm->tm_sec;
        }
    }

    if (UI_FINISHED_SHOWE_PRICE)
    {
        if ( UI_CONNECTOR_STATE(UI_CONNECTOR_ACTIVE_IDX) == UI_CONNECTOR_STATE_RESERVED )
        {
            if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE )
            {
                char buf[64] = {0};
                snprintf(buf, sizeof(buf), "%02d:%02d:%02d", show_hour, show_min, show_sec);
                ioc_label_set_text(ui_page_price_reserved.idle_time, buf);
            }
            else
            {
                ioc_label_set_text(ui_page_price_reserved.idle_time, ui_connector_get_text_time_value(UI_CONNECTOR_ACTIVE_IDX));
            }
        }
    }
}

// OCPPprice 价格体系在空闲时显示默认价格文本
static void ui_page_price_reserved_show_OCPPprie(bool is_show)
{
    if(is_show){
        lv_obj_clear_flag(ui_page_price_reserved.default_price_text, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(ui_page_price_reserved.curr_period, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_price_reserved.curr_price_kwh, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_price_reserved.curr_price_hr, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_price_reserved.curr_price_times, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_price_reserved.curr_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_price_reserved.curr_circle, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_price_reserved.split_line, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(ui_page_price_reserved.default_price_text, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_flag(ui_page_price_reserved.curr_period, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_price_reserved.curr_price_kwh, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_price_reserved.curr_price_hr, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_price_reserved.curr_price_times, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_price_reserved.curr_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_price_reserved.curr_circle, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_price_reserved.split_line, LV_OBJ_FLAG_HIDDEN);
    }
}

static void ui_page_price_reserved_show_price(bool is_show)
{

}

static void ui_page_price_reserved_updatejson(struct ioc_obj_user_data_s * user_data)
{
    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        return;
    }

    char buf[64] = {0};
    uint8_t startH = 0,  startM = 0, endH = 0, endM = 0;

    // 价格相关
    if(UI_RESERVED_SHOWE_PRICE)
    {
        // 判断是都要隐藏noprice
        if(s_reserved_show_price_flag != 1)
        {
            ui_page_price_reserved_show_price(true);
            s_reserved_show_price_flag = 1;
        }

        // 判断是否显示OCPPprice的默认价格文本
        if(UI_RESERVED_SHOWE_OCCPP_PRICE)
        {
            if(s_reserved_show_OCPPprice_flag != 1)
            {
                ui_page_price_reserved_show_OCPPprie(true);
                s_reserved_show_OCPPprice_flag = 1;
            }

            // 价格文本有变化时再更新
            if(strcmp(s_reserved_price_text, UI_RESERVED_DATA.currPrice.priceText) != 0)
            {
                strcpy(s_reserved_price_text, UI_RESERVED_DATA.currPrice.priceText);
                ioc_label_set_text(ui_page_price_reserved.default_price_text, s_reserved_price_text);
                LV_LOG_USER("Default Price Text [%s]", s_reserved_price_text);
            }

            return;
        }
        else
        {
            if(s_reserved_show_OCPPprice_flag != 0)
            {
                ui_page_price_reserved_show_OCPPprie(false);
                s_reserved_show_OCPPprice_flag = 0;
            }
        }

        // 设置时间段
        startH = (uint8_t)(UI_RESERVED_DATA.currPrice.startMin / 60);
        startM = (uint8_t)(UI_RESERVED_DATA.currPrice.startMin % 60);
        endH   = (uint8_t)(UI_RESERVED_DATA.nextPrice.startMin/ 60);
        endM   = (uint8_t)(UI_RESERVED_DATA.nextPrice.startMin % 60);

        if (ui_page_price_reserved.curr_period != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            sprintf((char*)buf, "%02d:%02d - %02d:%02d",
            startH, startM, endH, endM);
            ioc_label_set_text(ui_page_price_reserved.curr_period , buf);
        }

        // 根据costPriceDecimalPlaces判断保留几位小数
        char fmt[16];
        snprintf(fmt, sizeof(fmt), "%%.%df", g_ui_json.costPriceDecimalPlaces);

        if (ui_page_price_reserved.curr_price_kwh != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_RESERVED_DATA.currPrice.kWhPrice);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            strncat(buf, "/kWh", sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(ui_page_price_reserved.curr_price_kwh, buf);
        }

        if (ui_page_price_reserved.curr_price_hr != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_RESERVED_DATA.currPrice.hourPrice);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            strncat(buf, "/hr", sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(ui_page_price_reserved.curr_price_hr, buf);
        }
        if (ui_page_price_reserved.curr_price_times != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_RESERVED_DATA.currPrice.flatFee);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            strncat(buf, "/times", sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(ui_page_price_reserved.curr_price_times, buf);
        }
    }
    else
    {
        // 判断是否要隐藏info_card
        if(s_reserved_show_price_flag != 0)
        {
            ui_page_price_reserved_show_price(false);
            s_reserved_show_price_flag = 0;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
lv_obj_t * ui_page_create_price_reserved(lv_obj_t *parent)
{
    memset(&ui_page_price_reserved, 0, sizeof(ui_page_price_reserved));

    ui_page_price_reserved.page = ioc_layout_mngr_load_layout("reserved", parent);
    if (ui_page_price_reserved.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(ui_page_price_reserved.page);
        user_data->ui_json_update_cb = ui_page_price_reserved_updatejson;
    }

    return ui_page_price_reserved.page;
}

void ui_page_show_price_reserved(void)
{
    // 为了时间显示的流畅，占位时间由本地计算
    if(s_reserved_timer == NULL)
    {
        s_reserved_timer = lv_timer_create(ui_page_price_reserved_time, 500, NULL);
    }
}
void ui_page_hide_price_reserved(void)
{
    if (s_reserved_timer!= NULL)
    {
        lv_timer_del(s_reserved_timer);
        s_reserved_timer = NULL;
        char buf[64] = {0};
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", 0, 0, 0);
        ioc_label_set_text(ui_page_price_reserved.idle_time, buf);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ui_reserved_update_left_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_price_reserved.left_label, "Left");
}

void ui_after_create_price_reserved_left_label(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved.left_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_reserved_update_left_label;
}

void ui_reserved_update_plug(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_price_reserved.plug, "Plug_in_frist");
}

void ui_after_create_price_reserved_plug(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved.plug = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_reserved_update_plug;
}

void ui_reserved_update_current_time_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_price_reserved.current_time_label, "Current_Period");
}

void ui_after_create_price_reserved_current_time_label(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved.current_time_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_reserved_update_current_time_label;
}

void ui_after_create_price_reserved_idle_time(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved.idle_time = obj;
}
void ui_after_create_price_reserved_current_period(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved.curr_period = obj;
}
void ui_after_create_price_reserved_current_price_kWh(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved.curr_price_kwh = obj;
}
void ui_after_create_price_reserved_current_price_hr(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved.curr_price_hr = obj;
}
void ui_after_create_price_reserved_current_price_times(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved.curr_price_times = obj;
}
void ui_after_create_price_reserved_default_price_text(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved.default_price_text = obj;
}
void ui_after_create_price_reserved_current_img(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved.curr_img = obj;
}
void ui_after_create_price_reserved_current_circle(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved.curr_circle = obj;
}
void ui_after_create_price_reserved_split_line(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved.split_line = obj;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_after_create_price_reserved(const char * name, lv_obj_t * obj)
{
    const char * reserved_name = name + strlen(UI_NAME_PRICE_RESERVED);
    if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_LEFT_LABEL) == 0)
    {
        ui_after_create_price_reserved_left_label(name, obj);
        return;
    }
    else if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_PLUG) == 0)
    {
        ui_after_create_price_reserved_plug(name, obj);
        return;
    }
    else if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_CURRENT_TIME_LABEL) == 0)
    {
        ui_after_create_price_reserved_current_time_label(name, obj);
        return;
    }
    else if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_IDLE_TIME) == 0)
    {
        ui_after_create_price_reserved_idle_time(name, obj);
        return;
    }
    else if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_CURRENT_PERIOD) == 0)
    {
        ui_after_create_price_reserved_current_period(name, obj);
        return;
    }
    else if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_CURRNET_PRICE_KWH) == 0)
    {
        ui_after_create_price_reserved_current_price_kWh(name, obj);
        return;
    }    else if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_CURRNET_PRICE_HR) == 0)
    {
        ui_after_create_price_reserved_current_price_hr(name, obj);
        return;
    }
    else if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_CURRNET_PRICE_TIMES) == 0)
    {
        ui_after_create_price_reserved_current_price_times(name, obj);
        return;
    }
    else if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_DEFAULT_PRICE_TEXT) == 0)
    {
        ui_after_create_price_reserved_default_price_text(name, obj);
        return;
    }
    else if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_CURRENT_IMG) == 0)
    {
        ui_after_create_price_reserved_current_img(name, obj);
        return;
    }
    else if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_CURRENT_CIRCLE) == 0)
    {
        ui_after_create_price_reserved_current_circle(name, obj);
        return;
    }
    else if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_SPLIT_LINE) == 0)
    {
        ui_after_create_price_reserved_split_line(name, obj);
        return;
    }
}
