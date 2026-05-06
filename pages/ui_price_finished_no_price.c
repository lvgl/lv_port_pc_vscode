#include "../pages_mngr.h"


// 无价格体系信息框
#define UI_NAME_PRICE_FINISHED_IDLE_TIME_NOPRICE           "price_finished_time"
#define UI_NAME_PRICE_FINISHED_IDLE_TIME_LABEL_NOPRICE     "price_finished_title"
#define UI_NAME_PRICE_FINISHED_PROMPT_NOPRICE              "price_finished_tip"


#define UI_FINISHED_DATA  g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.finished
#define UI_FINISHED_SHOWE_PRICE g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].showPrice


typedef struct {

    lv_obj_t * page;

    // 无价格体系信息框
    lv_obj_t * idle_time_noprice_label;
    lv_obj_t * idle_time_noprice;
    lv_obj_t * prompt_noprice;

} ui_page_price_finished_no_price_t;

static ui_page_price_finished_no_price_t ui_page_price_finished = {NULL};
static lv_timer_t * s_finished_timer  = NULL;
static int s_finished_show_price_flag = -1;      // -1:未收到UI.jison时的初始值  0：不显示价格 1：显示价格


static void ui_page_price_finished_no_price_time(lv_timer_t * temp)
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
            ioc_label_set_text(ui_page_price_finished.idle_time_noprice, buf);
        }
        else
        {
            ioc_label_set_text(ui_page_price_finished.idle_time_noprice, ui_connector_get_text_time_value(UI_CONNECTOR_ACTIVE_IDX));
        }
    }
}

static void ui_page_06_finised_show_price(bool is_show)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void ui_page_price_finished_no_price_updatejson(struct ioc_obj_user_data_s * user_data)
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
lv_obj_t * ui_page_create_price_finished_no_price(lv_obj_t *parent)
{
    memset(&ui_page_price_finished, 0, sizeof(ui_page_price_finished));

    ui_page_price_finished.page = ioc_layout_mngr_load_layout("finished", parent);
    if (ui_page_price_finished.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(ui_page_price_finished.page);
        user_data->ui_json_update_cb = ui_page_price_finished_no_price_updatejson;
    }

    return ui_page_price_finished.page;
}

static void ui_page_show_price_finished_no_price(void)
{
    // 为了时间显示的流畅，占位时间由本地计算
    if(s_finished_timer == NULL)
    {
        s_finished_timer = lv_timer_create(ui_page_price_finished_no_price_time, 500, NULL);
    }
}

static void ui_page_hide_price_finished_no_price(void)
{
    if (s_finished_timer!= NULL)
    {
        lv_timer_del(s_finished_timer);
        s_finished_timer = NULL;
        char buf[64] = {0};
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", 0, 0, 0);
        ioc_label_set_text(ui_page_price_finished.idle_time_noprice, buf);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ui_finished_update_prompt(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_price_finished.prompt_noprice, "Idle_time_prompt");
}

void ui_after_create_price_finished_no_price_prompt_noprice(const char * name, lv_obj_t * obj)
{
    ui_page_price_finished.prompt_noprice = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_finished_update_prompt;
}

void ui_finished_update_idle_time_label_noprice(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_price_finished.idle_time_noprice_label, "Idle_time");
}

void ui_after_create_price_finished_no_price_idle_time_label_noprice(const char * name, lv_obj_t * obj)
{
    ui_page_price_finished.idle_time_noprice_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_finished_update_idle_time_label_noprice;
}

void ui_after_create_price_finished_no_price_idle_time_noprice(const char * name, lv_obj_t * obj)
{
    ui_page_price_finished.idle_time_noprice = obj;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_after_create_price_finished_no_price(const char * name, lv_obj_t * obj)
{
    const char * finished_name = name + strlen(UI_NAME_PRICE_FINISHED);
    if (strcmp(finished_name, UI_NAME_PRICE_FINISHED_IDLE_TIME_NOPRICE) == 0)
    {
        ui_after_create_price_finished_no_price_idle_time_noprice(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_PRICE_FINISHED_PROMPT_NOPRICE) == 0)
    {
        ui_after_create_price_finished_no_price_prompt_noprice(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_PRICE_FINISHED_IDLE_TIME_LABEL_NOPRICE) == 0)
    {
        ui_after_create_price_finished_no_price_idle_time_label_noprice(name, obj);
        return;
    }
}
