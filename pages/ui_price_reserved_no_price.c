#include "../pages_mngr.h"

// 无价格体系信息框
#define UI_NAME_PRICE_RESERVED_PRICE_LEFT_TIME_LABEL             "price_reserved_title"
#define UI_NAME_PRICE_RESERVED_PRICE_PLUG_NOPRICE                "price_reserved_tip"
#define UI_NAME_PRICE_RESERVED_PRICE_IDLE_TIME_NOPRICE           "price_reserved_time"

typedef struct {

    lv_obj_t * page;

    lv_obj_t * left_time_label;
    lv_obj_t * plug_noprice;
    lv_obj_t * info_card_idle_time_noprice;

} ui_page_price_reserved_no_price_t;

static ui_page_price_reserved_no_price_t ui_page_price_reserved_no_price = {NULL};

lv_timer_t * s_reserved_no_price_timer = NULL;

static void ui_page_price_reserved_no_price_time(lv_timer_t * temp)
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

    if ( UI_CONNECTOR_STATE(UI_CONNECTOR_ACTIVE_IDX) == UI_CONNECTOR_STATE_RESERVED )
    {
        if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE )
        {
            char buf[64] = {0};
            snprintf(buf, sizeof(buf), "%02d:%02d:%02d", show_hour, show_min, show_sec);
            ioc_label_set_text(ui_page_price_reserved_no_price.info_card_idle_time_noprice, buf);
        }
        else
        {
            ioc_label_set_text(ui_page_price_reserved_no_price.info_card_idle_time_noprice, ui_connector_get_text_time_value(UI_CONNECTOR_ACTIVE_IDX));
        }
    }
}

static void ui_page_price_reserved_no_price_show_price(bool is_show)
{

}

static void ui_page_price_reserved_no_price_updatejson(struct ioc_obj_user_data_s * user_data)
{
    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        return;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
lv_obj_t * ui_page_create_price_reserved_no_price(lv_obj_t *parent)
{
    memset(&ui_page_price_reserved_no_price, 0, sizeof(ui_page_price_reserved_no_price));

    ui_page_price_reserved_no_price.page = ioc_layout_mngr_load_layout("reserved", parent);
    if (ui_page_price_reserved_no_price.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(ui_page_price_reserved_no_price.page);
        user_data->ui_json_update_cb = ui_page_price_reserved_no_price_updatejson;
    }

    return ui_page_price_reserved_no_price.page;
}

void ui_page_show_price_reserved_no_price(void)
{
    // 为了时间显示的流畅，占位时间由本地计算
    if(s_reserved_no_price_timer == NULL)
    {
        s_reserved_no_price_timer = lv_timer_create(ui_page_price_reserved_no_price_time, 500, NULL);
    }
}
void ui_page_hide_price_reserved_no_price(void)
{
    if (s_reserved_no_price_timer!= NULL)
    {
        lv_timer_del(s_reserved_no_price_timer);
        s_reserved_no_price_timer = NULL;
        char buf[64] = {0};
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", 0, 0, 0);
        ioc_label_set_text(ui_page_price_reserved_no_price.info_card_idle_time_noprice, buf);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_reserved_update_left_time_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_price_reserved_no_price.left_time_label, "Left");
}

void ui_after_create_price_reserved_no_price_left_time_label(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved_no_price.left_time_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_reserved_update_left_time_label;
}

void ui_reserved_update_plug_noprice(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_price_reserved_no_price.plug_noprice, "Plug_in_frist");
}

void ui_after_create_price_reserved_no_price_plug_noprice(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved_no_price.plug_noprice = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_reserved_update_plug_noprice;
}

void ui_after_create_price_reserved_no_price_idle_time_noprice(const char * name, lv_obj_t * obj)
{
    ui_page_price_reserved_no_price.info_card_idle_time_noprice = obj;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_after_create_price_reserved_no_price(const char * name, lv_obj_t * obj)
{
    const char * reserved_name = name + strlen(UI_NAME_PRICE_RESERVED_NO_PRICE);
    if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_PRICE_LEFT_TIME_LABEL) == 0)
    {
        ui_after_create_price_reserved_no_price_left_time_label(name, obj);
        return;
    }
    else if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_PRICE_PLUG_NOPRICE) == 0)
    {
        ui_after_create_price_reserved_no_price_plug_noprice(name, obj);
        return;
    }
    else if (strcmp(reserved_name, UI_NAME_PRICE_RESERVED_PRICE_IDLE_TIME_NOPRICE) == 0)
    {
        ui_after_create_price_reserved_no_price_idle_time_noprice(name, obj);
        return;
    }
}
