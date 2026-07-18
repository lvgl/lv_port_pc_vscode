#include "../pages_mngr.h"

#define UI_NAME_PRICE_FINISHED_IDLE_TIME                   "price_finished_time"
#define UI_NAME_PRICE_FINISHED_IDLE_TIME_LABEL             "price_finished_title"
#define UI_NAME_PRICE_FINISHED_IDLE_TIME_PROMPT            "price_finished_tip"
#define UI_NAME_PRICE_FINISHED_CHARGING_COST_LABEL         "price_finished_charging_title"
#define UI_NAME_PRICE_FINISHED_IDLE_FEE_LABEL              "price_finished_idle_title"
#define UI_NAME_PRICE_FINISHED_TOTAL_AMOUNT_LABEL          "price_finished_total_title"
#define UI_NAME_PRICE_FINISHED_CHARGING_COST               "price_finished_charging_val"
#define UI_NAME_PRICE_FINISHED_IDLE_FEE                    "price_finished_idle_val"
#define UI_NAME_PRICE_FINISHED_TOTAL_AMOUNT                "price_finished_total_val"


#define UI_FINISHED_DATA  g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.finished
#define UI_FINISHED_SHOWE_PRICE g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].showPrice


typedef struct {

    lv_obj_t * page;

    lv_obj_t * idle_time;
    lv_obj_t * charging_cost;
    lv_obj_t * idle_fee;
    lv_obj_t * total_amount;

    lv_obj_t * idle_time_label;
    lv_obj_t * idle_time_prompt;
    lv_obj_t * charging_cost_label;
    lv_obj_t * idle_fee_label;
    lv_obj_t * total_amount_label;


} ui_page_price_finished_t;

static ui_page_price_finished_t ui_page_price_finished = {NULL};
static lv_timer_t * s_finished_timer  = NULL;
static int s_finished_show_price_flag = -1;      // -1:未收到UI.jison时的初始值  0：不显示价格 1：显示价格


static void ui_page_price_finished_time(lv_timer_t * temp)
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
        time_t diftimer = difftime(current_time, UI_FINISHED_DATA.stopTime);

        if (diftimer < 0) {
            diftimer = 0;
        }

        struct tm *tm = gmtime(&diftimer);


        if (current_time >= UI_FINISHED_DATA.stopTime) {
            show_hour = tm->tm_hour;
            show_min  = tm->tm_min;
            show_sec  = tm->tm_sec;
        }
    }

    if ( UI_CONNECTOR_STATE(UI_CONNECTOR_ACTIVE_IDX) == UI_CONNECTOR_STATE_FINISHED )
    {
        if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE )
        {
            char buf[64] = {0};
            snprintf(buf, sizeof(buf), "%02d:%02d:%02d", show_hour, show_min, show_sec);
            ioc_label_set_text(ui_page_price_finished.idle_time, buf);
        }
        else
        {
            ioc_label_set_text(ui_page_price_finished.idle_time, ui_connector_get_text_time_value(UI_CONNECTOR_ACTIVE_IDX));
        }
    }
}

static void ui_page_06_finised_show_price(bool is_show)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void ui_page_price_finished_updatejson(struct ioc_obj_user_data_s * user_data)
{
    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        return;
    }

    char buf[64] = {0};


    // 价格相关
    if(UI_FINISHED_SHOWE_PRICE)
    {
        if(s_finished_show_price_flag != 1)
        {
            ui_page_06_finised_show_price(true);
            s_finished_show_price_flag = 1;
        }


        // 根据costPriceDecimalPlaces判断保留几位小数
        char fmt[16];
        snprintf(fmt, sizeof(fmt), "%%.%df", g_ui_json.costPriceDecimalPlaces);

        if (ui_page_price_finished.charging_cost != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            double chargingCost = 0;
            if(UI_FINISHED_DATA.cost.totalCost > UI_FINISHED_DATA.cost.stopIdleCost)
            {
                chargingCost = UI_FINISHED_DATA.cost.totalCost - UI_FINISHED_DATA.cost.stopIdleCost;
            }
            snprintf(buf, sizeof(buf), fmt, chargingCost);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(ui_page_price_finished.charging_cost, buf);
        }
        if (ui_page_price_finished.idle_fee != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_FINISHED_DATA.cost.stopIdleCost);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(ui_page_price_finished.idle_fee, buf);
        }
        if (ui_page_price_finished.total_amount != NULL)
        {
            memset(buf, 0x00, sizeof(buf));
            snprintf(buf, sizeof(buf), fmt, UI_FINISHED_DATA.cost.totalCost);
            strncat(buf, g_ui_json.currencyIcon, sizeof(buf) - strlen(buf) - 1);
            ioc_label_set_text(ui_page_price_finished.total_amount, buf);
        }
    }
    else
    {
        if(s_finished_show_price_flag != 0)
        {
            ui_page_06_finised_show_price(false);
            s_finished_show_price_flag = 0;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
lv_obj_t * ui_page_create_price_finished(lv_obj_t *parent)
{
    memset(&ui_page_price_finished, 0, sizeof(ui_page_price_finished));

    ui_page_price_finished.page = ioc_layout_mngr_load_layout("finished", parent);
    if (ui_page_price_finished.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(ui_page_price_finished.page);
        user_data->ui_json_update_cb = ui_page_price_finished_updatejson;
    }

    return ui_page_price_finished.page;
}

void ui_page_show_price_finished(void)
{
    // 为了时间显示的流畅，占位时间由本地计算
    if(s_finished_timer == NULL)
    {
        s_finished_timer = lv_timer_create(ui_page_price_finished_time, 500, NULL);
    }
}

void ui_page_hide_price_finished(void)
{
    if (s_finished_timer!= NULL)
    {
        lv_timer_del(s_finished_timer);
        s_finished_timer = NULL;
        char buf[64] = {0};
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", 0, 0, 0);
        ioc_label_set_text(ui_page_price_finished.idle_time, buf);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_finished_update_idle_time_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_price_finished.idle_time_label, "Idle_time");
}

void ui_after_create_price_finished_idle_time_label(const char * name, lv_obj_t * obj)
{
    ui_page_price_finished.idle_time_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_finished_update_idle_time_label;
}

void ui_finished_update_idle_time_prompt(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_price_finished.idle_time_prompt, "Idle_time_prompt");
}

void ui_after_create_price_finished_idle_time_prompt(const char * name, lv_obj_t * obj)
{
    ui_page_price_finished.idle_time_prompt = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_finished_update_idle_time_prompt;
}

void ui_finished_update_charging_cost_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_price_finished.charging_cost_label, "Charging_cost");
}

void ui_after_create_price_finished_charging_cost_label(const char * name, lv_obj_t * obj)
{
    ui_page_price_finished.charging_cost_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_finished_update_charging_cost_label;
}

void ui_finished_update_idle_fee_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_price_finished.idle_fee_label, "Idle_fee");
}

void ui_after_create_price_finished_idle_fee_label(const char * name, lv_obj_t * obj)
{
    ui_page_price_finished.idle_fee_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_finished_update_idle_fee_label;
}

void ui_finished_update_total_amount_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_price_finished.total_amount_label, "Total_Amount");
}

void ui_after_create_price_finished_total_amount_label(const char * name, lv_obj_t * obj)
{
    ui_page_price_finished.total_amount_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_finished_update_total_amount_label;
}


void ui_after_create_price_finished_idle_time(const char * name, lv_obj_t * obj)
{
    ui_page_price_finished.idle_time = obj;
}
void ui_after_create_price_finished_charging_cost(const char * name, lv_obj_t * obj)
{
    ui_page_price_finished.charging_cost = obj;
}
void ui_after_create_price_finished_idle_fee(const char * name, lv_obj_t * obj)
{
    ui_page_price_finished.idle_fee = obj;
}
void ui_after_create_price_finished_total_amount(const char * name, lv_obj_t * obj)
{
    ui_page_price_finished.total_amount = obj;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_after_create_price_finished(const char * name, lv_obj_t * obj)
{
    const char * finished_name = name + strlen(UI_NAME_PRICE_FINISHED);
    if (strcmp(finished_name, UI_NAME_PRICE_FINISHED_IDLE_TIME) == 0)
    {
        ui_after_create_price_finished_idle_time(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_PRICE_FINISHED_IDLE_TIME_LABEL) == 0)
    {
        ui_after_create_price_finished_idle_time_label(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_PRICE_FINISHED_IDLE_TIME_PROMPT) == 0)
    {
        ui_after_create_price_finished_idle_time_prompt(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_PRICE_FINISHED_CHARGING_COST_LABEL) == 0)
    {
        ui_after_create_price_finished_charging_cost_label(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_PRICE_FINISHED_IDLE_FEE_LABEL) == 0)
    {
        ui_after_create_price_finished_idle_fee_label(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_PRICE_FINISHED_TOTAL_AMOUNT_LABEL) == 0)
    {
        ui_after_create_price_finished_total_amount_label(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_PRICE_FINISHED_CHARGING_COST) == 0)
    {
        ui_after_create_price_finished_charging_cost(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_PRICE_FINISHED_IDLE_FEE) == 0)
    {
        ui_after_create_price_finished_idle_fee(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_PRICE_FINISHED_TOTAL_AMOUNT) == 0)
    {
        ui_after_create_price_finished_total_amount(name, obj);
        return;
    }
}
