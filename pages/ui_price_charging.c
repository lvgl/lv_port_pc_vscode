#include "../pages_mngr.h"

#define UI_CHARGING_DATA  g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.charging
#define UI_CHARGING_SHOWE_PRICE g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].showPrice

#define UI_NAME_PRICE_CHARGING_PRICE_KWH          ".price_charging_current_rate.price_kwh"
#define UI_NAME_PRICE_CHARGING_PRICE_HR           ".price_charging_current_rate.price_hr"
#define UI_NAME_PRICE_CHARGING_PRICE_TIMES        ".price_charging_current_rate.price_times"

#define UI_NAME_PRICE_CHARGING_COST_LABEL          ".cost_charging_title"
#define UI_NAME_PRICE_CHARGING_COST                ".cost_charging_val"

#define UI_NAME_PRICE_CHARGING_BTN                 ".detaile_btn"
#define UI_NAME_PRICE_CHARGING_BTN_LABEL           ".detaile_btn.label"


typedef struct {

    lv_obj_t * page;

    lv_obj_t * price_kwh;
    lv_obj_t * price_hr;
    lv_obj_t * price_times;
    lv_obj_t * cost;
    lv_obj_t * estimated_cost;

    lv_obj_t * btn;
    lv_obj_t * btn_label;

} ui_page_price_charging_t;

static ui_page_price_charging_t s_ui_page_price_charging = {NULL};
static lv_timer_t * charging_timer = NULL;
static void ui_page_price_charging_show_price(bool is_show)
{
    lv_obj_t * objs[] = {
        s_ui_page_price_charging.price_kwh,
        s_ui_page_price_charging.price_hr,
        s_ui_page_price_charging.price_times,
        s_ui_page_price_charging.cost,
        s_ui_page_price_charging.estimated_cost,
        s_ui_page_price_charging.btn,
        s_ui_page_price_charging.btn_label,
    };

    for (uint32_t i = 0; i < sizeof(objs) / sizeof(objs[0]); i++)
    {
        if (objs[i] == NULL) continue;
        if (is_show) lv_obj_clear_flag(objs[i], LV_OBJ_FLAG_HIDDEN);
        else lv_obj_add_flag(objs[i], LV_OBJ_FLAG_HIDDEN);
    }
}
static int s_charging_show_price_flag = -1;      // -1:未收到UI.jison时的初始值  0：不显示价格 1：显示价格



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void ui_page_price_charging_updatejson(struct ioc_obj_user_data_s * user_data)
{
    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        return;
    }

    char buf[64] = {0};

    // 价格相关
    if(UI_CHARGING_SHOWE_PRICE)
    {
        if(s_charging_show_price_flag != 1)
        {
            ui_page_price_charging_show_price(true);
            s_charging_show_price_flag = 1;
        }

        // 根据costPriceDecimalPlaces判断保留几位小数
        char fmt[16];
        snprintf(fmt, sizeof(fmt), "%%.%df", g_ui_json.costPriceDecimalPlaces);

        if (s_ui_page_price_charging.price_kwh != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_CHARGING_DATA.currPrice.kWhPrice);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            strncat(buf, "/kWh", sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(s_ui_page_price_charging.price_kwh, buf);
        }
        if (s_ui_page_price_charging.price_hr != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_CHARGING_DATA.currPrice.hourPrice);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            strncat(buf, "/hr", sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(s_ui_page_price_charging.price_hr, buf);
        }
        if (s_ui_page_price_charging.price_times != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_CHARGING_DATA.currPrice.flatFee);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            strncat(buf, "/times", sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(s_ui_page_price_charging.price_times, buf);
        }
        if (s_ui_page_price_charging.cost != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_CHARGING_DATA.cost.totalCost);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(s_ui_page_price_charging.cost, buf);
        }
    }
    else
    {
        if(s_charging_show_price_flag != 0)
        {
            ui_page_price_charging_show_price(false);
            s_charging_show_price_flag = 0;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
lv_obj_t * ui_page_create_price_charging(lv_obj_t *parent)
{
    memset(&s_ui_page_price_charging, 0, sizeof(s_ui_page_price_charging));

    s_ui_page_price_charging.page = ioc_layout_mngr_load_layout("price_charging", parent);
    if (s_ui_page_price_charging.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(s_ui_page_price_charging.page);
        user_data->ui_json_update_cb = ui_page_price_charging_updatejson;
    }

    return s_ui_page_price_charging.page;
}

void ui_page_show_price_charging(void)
{

}

void ui_page_hide_price_charging(void)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_after_create_price_charging_price_kwh(const char * name, lv_obj_t * obj)
{
    s_ui_page_price_charging.price_kwh = obj;
}
void ui_after_create_price_charging_price_hr(const char * name, lv_obj_t * obj)
{
    s_ui_page_price_charging.price_hr = obj;
}
void ui_after_create_price_charging_price_times(const char * name, lv_obj_t * obj)
{
    s_ui_page_price_charging.price_times = obj;
}
void ui_after_create_price_charging_cost(const char * name, lv_obj_t * obj)
{
    s_ui_page_price_charging.cost = obj;
}

void ui_charging_update_estimated_cost(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(s_ui_page_price_charging.estimated_cost, "Estimated_Cost");
}

void ui_after_create_price_charging_estimated_cost(const char * name, lv_obj_t * obj)
{
    s_ui_page_price_charging.estimated_cost = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_charging_update_estimated_cost;
}


static void charging_detailed_data_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        ui_popup_show_charging_detail(0,0);
    }
}

void ui_after_create_price_charging_btn(const char * name, lv_obj_t * obj)
{
    s_ui_page_price_charging.btn = obj;
    lv_obj_add_event_cb(obj, charging_detailed_data_event_cb, LV_EVENT_CLICKED, NULL);
}

void ui_charging_update_btn_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(s_ui_page_price_charging.btn_label, "Detailed_data");
}

void ui_after_create_price_charging_btn_label(const char * name, lv_obj_t * obj)
{
    s_ui_page_price_charging.btn_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_charging_update_btn_label;
}



static void charging_detailed_data_noprice_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        ui_popup_show_charging_detail(0,0);
    }
}

void ui_after_create_price_charging(const char * name, lv_obj_t * obj)
{
    const char * charging_name = name + strlen(UI_NAME_PRICE_CHARGING);
    if (strcmp(charging_name, UI_NAME_PRICE_CHARGING_COST_LABEL) == 0)
    {
        ui_after_create_price_charging_estimated_cost(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_PRICE_CHARGING_BTN) == 0)
    {
        ui_after_create_price_charging_btn(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_PRICE_CHARGING_BTN_LABEL) == 0)
    {
        ui_after_create_price_charging_btn_label(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_PRICE_CHARGING_PRICE_KWH) == 0)
    {
        ui_after_create_price_charging_price_kwh(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_PRICE_CHARGING_PRICE_HR) == 0)
    {
        ui_after_create_price_charging_price_hr(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_PRICE_CHARGING_PRICE_TIMES) == 0)
    {
        ui_after_create_price_charging_price_times(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_PRICE_CHARGING_COST) == 0)
    {
        ui_after_create_price_charging_cost(name, obj);
        return;
    }


    return;
}
