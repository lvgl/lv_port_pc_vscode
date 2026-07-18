#include "../pages_mngr.h"

#define UI_IDLE_DATA  g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.idle
#define UI_IDLE_SHOWE_PRICE g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].showPrice
#define UI_IDLE_SHOWE_OCCPP_PRICE g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].isOcppPrice

#define UI_NAME_PRICE_IDLE_CURRENT_PERIOD_LABEL    ".price_idle_current.price_current_title"
#define UI_NAME_PRICE_IDLE_CURRENT_PERIOD          ".price_idle_current.price_current_time"
#define UI_NAME_PRICE_IDLE_CURRENT_IMG             ".price_idle_current.price_current_period_icon"
#define UI_NAME_PRICE_IDLE_CURRENT_CIRCLE          ".price_idle_current.price_current_dot"
#define UI_NAME_PRICE_IDLE_CURRNET_PRICE_KWH       ".price_idle_current.price_current_rate.price_kwh"
#define UI_NAME_PRICE_IDLE_CURRNET_PRICE_HR        ".price_idle_current.price_current_rate.price_hr"
#define UI_NAME_PRICE_IDLE_CURRNET_PRICE_TIMES     ".price_idle_current.price_current_rate.price_times"

#define UI_NAME_PRICE_IDLE_SPLIT_LINE              ".price_idle_divider"

#define UI_NAME_PRICE_IDLE_NEXT_PERIOD_LABEL       ".price_idle_next.price_next_title"
#define UI_NAME_PRICE_IDLE_NEXT_PERIOD             ".price_idle_next.price_next_time"
#define UI_NAME_PRICE_IDLE_NEXT_IMG                ".price_idle_next.price_next_period_img"
#define UI_NAME_PRICE_IDLE_NEXT_CIRCLE             ".price_idle_next.price_next_dot"
#define UI_NAME_PRICE_IDLE_NEXT_PRICE_KWH          ".price_idle_next.price_next_rate.price_kwh"
#define UI_NAME_PRICE_IDLE_NEXT_PRICE_HR           ".price_idle_next.price_next_rate.price_hr"
#define UI_NAME_PRICE_IDLE_NEXT_PRICE_TIMES        ".price_idle_next.price_next_rate.price_times"


#define UI_NAME_PRICE_IDLE_DEFAULT_PRICE_TEXT      ".ocpp_price"


typedef struct {

    lv_obj_t * page;

    lv_obj_t * curr_period;
    lv_obj_t * curr_price_kwh;
    lv_obj_t * curr_price_hr;
    lv_obj_t * curr_price_times;
    lv_obj_t * next_period;
    lv_obj_t * next_price_kwh;
    lv_obj_t * next_price_hr;
    lv_obj_t * next_price_times;
    lv_obj_t * default_price_text;
    lv_obj_t * curr_img;
    lv_obj_t * next_img;
    lv_obj_t * curr_circle;
    lv_obj_t * next_circle;
    lv_obj_t * split_line;

    lv_obj_t * curr_period_label;
    lv_obj_t * next_period_label;

} ui_page_PRICE_IDLE_t;

static ui_page_PRICE_IDLE_t ui_page_idle_price = {NULL};
static int s_idle_show_price_flag = -1;      // -1:未收到UI.jison时的初始值  0：不显示价格 1：显示价格
static int s_idle_show_OCPPprice_flag = -1;  // -1:未收到UI.jison时的初始值  0：显示正常价格 1：显示OCPP默认价格文本
static char s_idle_price_text[256] = {0};


// OCPPprice 价格体系在空闲时显示默认价格文本
static void ui_page_price_idle_show_OCPPprie(bool is_show)
{
    if(is_show){
        lv_obj_clear_flag(ui_page_idle_price.default_price_text, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(ui_page_idle_price.curr_period, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.curr_price_kwh, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.curr_price_hr, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.curr_price_times, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.next_period, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.next_price_kwh, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.next_price_hr, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.next_price_times, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.curr_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.next_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.curr_circle, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.next_circle, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.split_line, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.curr_period_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_page_idle_price.next_period_label, LV_OBJ_FLAG_HIDDEN);

    } else {
        lv_obj_add_flag(ui_page_idle_price.default_price_text, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_flag(ui_page_idle_price.curr_period, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.curr_price_kwh, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.curr_price_hr, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.curr_price_times, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.next_period, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.next_price_kwh, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.next_price_hr, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.next_price_times, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.curr_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.next_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.curr_circle, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.next_circle, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.split_line, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.curr_period_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_page_idle_price.next_period_label, LV_OBJ_FLAG_HIDDEN);
    }
}


static void ui_page_PRICE_IDLE_updatejson(struct ioc_obj_user_data_s * user_data)
{
    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        return;
    }

    char buf[64] = {0};
    uint8_t startH = 0,  startM = 0, endH = 0, endM = 0;

    // 价格相关
    if(UI_IDLE_SHOWE_PRICE)
    {
        // 判断是都要隐藏noprice
        // if(s_idle_show_price_flag != 1)
        // {
        //     ui_page_PRICE_IDLE_show_price(true);
        //     s_idle_show_price_flag = 1;
        // }

        // 判断是否显示OCPPprice的默认价格文本
        if(UI_IDLE_SHOWE_OCCPP_PRICE)
        {
            if(s_idle_show_OCPPprice_flag != 1)
            {
                ui_page_price_idle_show_OCPPprie(true);
                s_idle_show_OCPPprice_flag = 1;
            }

            // 价格文本有变化时再更新
            if(strcmp(s_idle_price_text, UI_IDLE_DATA.currPrice.priceText) != 0)
            {
                strcpy(s_idle_price_text, UI_IDLE_DATA.currPrice.priceText);
                ioc_label_set_text(ui_page_idle_price.default_price_text, s_idle_price_text);
                LV_LOG_USER("Default Price Text [%s]", s_idle_price_text);
            }

            return;
        }
        else
        {
            if(s_idle_show_OCPPprice_flag != 0)
            {
                ui_page_price_idle_show_OCPPprie(false);
                s_idle_show_OCPPprice_flag = 0;
            }
        }

        // 设置时间段
        startH = (uint8_t)(UI_IDLE_DATA.currPrice.startMin / 60);
        startM = (uint8_t)(UI_IDLE_DATA.currPrice.startMin % 60);
        endH   = (uint8_t)(UI_IDLE_DATA.nextPrice.startMin/ 60);
        endM   = (uint8_t)(UI_IDLE_DATA.nextPrice.startMin % 60);

        if (ui_page_idle_price.curr_period != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            sprintf((char*)buf, "%02d:%02d - %02d:%02d",
            startH, startM, endH, endM);
            ioc_label_set_text(ui_page_idle_price.curr_period , buf);
        }

        startH = (uint8_t)(UI_IDLE_DATA.nextPrice.startMin / 60);
        startM = (uint8_t)(UI_IDLE_DATA.nextPrice.startMin % 60);
        endH   = (uint8_t)(UI_IDLE_DATA.nextPrice.endMin/ 60);
        endM   = (uint8_t)(UI_IDLE_DATA.nextPrice.endMin % 60);
        if (ui_page_idle_price.next_period != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            sprintf((char*)buf, "%02d:%02d - %02d:%02d",
            startH, startM, endH, endM);
            ioc_label_set_text(ui_page_idle_price.next_period , buf);
        }

        // 根据costPriceDecimalPlaces判断保留几位小数
        char fmt[16];
        snprintf(fmt, sizeof(fmt), "%%.%df", g_ui_json.costPriceDecimalPlaces);

        if (ui_page_idle_price.curr_price_kwh != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_IDLE_DATA.currPrice.kWhPrice);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            strncat(buf, "/kWh", sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(ui_page_idle_price.curr_price_kwh, buf);
        }

        if (ui_page_idle_price.curr_price_hr != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_IDLE_DATA.currPrice.hourPrice);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            strncat(buf, "/hr", sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(ui_page_idle_price.curr_price_hr, buf);
        }
        if (ui_page_idle_price.curr_price_times != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_IDLE_DATA.currPrice.flatFee);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            strncat(buf, "/times", sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(ui_page_idle_price.curr_price_times, buf);
        }
        if (ui_page_idle_price.next_price_kwh != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_IDLE_DATA.nextPrice.kWhPrice);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            strncat(buf, "/kWh", sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(ui_page_idle_price.next_price_kwh, buf);
        }
        if (ui_page_idle_price.next_price_hr != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_IDLE_DATA.nextPrice.hourPrice);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            strncat(buf, "/hr", sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(ui_page_idle_price.next_price_hr, buf);
        }
        if (ui_page_idle_price.next_price_times != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_IDLE_DATA.nextPrice.flatFee);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            strncat(buf, "/times", sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(ui_page_idle_price.next_price_times, buf);
        }
    }
    else
    {
        // // 判断是否要隐藏info_card
        // if(s_idle_show_price_flag != 0)
        // {
        //     ui_page_PRICE_IDLE_show_price(false);
        //     s_idle_show_price_flag = 0;
        // }
    }
}

lv_obj_t * ui_page_create_price_idle(lv_obj_t *parent)
{
    memset(&ui_page_idle_price, 0, sizeof(ui_page_idle_price));

    ui_page_idle_price.page = ioc_layout_mngr_load_layout("idle", parent);
    if (ui_page_idle_price.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(ui_page_idle_price.page);
        user_data->ui_json_update_cb = ui_page_PRICE_IDLE_updatejson;
    }

    return ui_page_idle_price.page;
}


void ui_idle_update_current_period_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_idle_price.curr_period_label, "Current_Period");
}

void ui_after_create_PRICE_IDLE_current_period_label(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.curr_period_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_idle_update_current_period_label;
}

void ui_idle_update_next_period_lable(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_idle_price.next_period_label, "Next_Period");
}

void ui_after_create_PRICE_IDLE_next_period_lable(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.next_period_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_idle_update_next_period_lable;
}

void ui_after_create_PRICE_IDLE_current_period(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.curr_period = obj;
}
void ui_after_create_PRICE_IDLE_current_price_kwh(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.curr_price_kwh = obj;
}
void ui_after_create_PRICE_IDLE_current_price_hr(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.curr_price_hr = obj;
}
void ui_after_create_PRICE_IDLE_current_price_times(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.curr_price_times = obj;
}
void ui_after_create_PRICE_IDLE_next_period(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.next_period = obj;
}
void ui_after_create_PRICE_IDLE_next_price_kwh(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.next_price_kwh = obj;
}
void ui_after_create_PRICE_IDLE_next_price_hr(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.next_price_hr = obj;
}
void ui_after_create_PRICE_IDLE_next_price_times(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.next_price_times = obj;
}
void ui_after_create_PRICE_IDLE_default_price_text(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.default_price_text = obj;
}
void ui_after_create_PRICE_IDLE_current_img(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.curr_img = obj;
}
void ui_after_create_PRICE_IDLE_current_circle(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.curr_circle = obj;
}
void ui_after_create_PRICE_IDLE_next_img(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.next_img = obj;
}
void ui_after_create_PRICE_IDLE_next_circle(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.next_circle = obj;
}
void ui_after_create_PRICE_IDLE_split_line(const char * name, lv_obj_t * obj)
{
    ui_page_idle_price.split_line = obj;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void ui_after_create_price_idle(const char * name, lv_obj_t * obj)
{
    const char * price_idle_name = name + strlen(UI_NAME_PRICE_IDLE);
    if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_CURRENT_PERIOD_LABEL) == 0)
    {
        ui_after_create_PRICE_IDLE_current_period_label(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_NEXT_PERIOD_LABEL) == 0)
    {
        ui_after_create_PRICE_IDLE_next_period_lable(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_CURRENT_PERIOD) == 0)
    {
        ui_after_create_PRICE_IDLE_current_period(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_CURRNET_PRICE_KWH) == 0)
    {
        ui_after_create_PRICE_IDLE_current_price_kwh(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_CURRNET_PRICE_HR) == 0)
    {
        ui_after_create_PRICE_IDLE_current_price_hr(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_CURRNET_PRICE_TIMES) == 0)
    {
        ui_after_create_PRICE_IDLE_current_price_times(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_NEXT_PERIOD) == 0)
    {
        ui_after_create_PRICE_IDLE_next_period(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_NEXT_PRICE_KWH) == 0)
    {
        ui_after_create_PRICE_IDLE_next_price_kwh(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_NEXT_PRICE_HR) == 0)
    {
        ui_after_create_PRICE_IDLE_next_price_hr(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_NEXT_PRICE_TIMES) == 0)
    {
        ui_after_create_PRICE_IDLE_next_price_times(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_DEFAULT_PRICE_TEXT) == 0)
    {
        ui_after_create_PRICE_IDLE_default_price_text(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_CURRENT_IMG) == 0)
    {
        ui_after_create_PRICE_IDLE_current_img(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_CURRENT_CIRCLE) == 0)
    {
        ui_after_create_PRICE_IDLE_current_circle(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_NEXT_IMG) == 0)
    {
        ui_after_create_PRICE_IDLE_next_img(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_NEXT_CIRCLE) == 0)
    {
        ui_after_create_PRICE_IDLE_next_circle(name, obj);
        return;
    }
    else if (strcmp(price_idle_name, UI_NAME_PRICE_IDLE_SPLIT_LINE) == 0)
    {
        ui_after_create_PRICE_IDLE_split_line(name, obj);
        return;
    }
}
