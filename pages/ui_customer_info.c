#include "../pages_mngr.h"

#define UI_NAME_CUSTOMER_INFO_QR_CODE_PROMPT                   "customer_hint"
#define UI_NAME_CUSTOMER_INFO_CHARGER_STATION                  "customer_charger_id"

typedef struct {
    lv_obj_t * page;

    lv_obj_t * qr_code_prompt;
    lv_obj_t * charger_station;

} ui_page_customer_info_t;

static ui_page_customer_info_t ui_page_customer_info = {NULL};

static void ui_page_customer_info_updatejson(struct ioc_obj_user_data_s * user_data)
{
    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        return;
    }

    char buf[64] = {0};
    uint8_t startH = 0,  startM = 0, endH = 0, endM = 0;
}

lv_obj_t * ui_page_create_customer_info(lv_obj_t *parent)
{

    ui_page_customer_info.page = ioc_layout_mngr_load_layout("cuestomer_info", parent);
    if (ui_page_customer_info.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(ui_page_customer_info.page);
        user_data->ui_json_update_cb = ui_page_customer_info_updatejson;
    }

    return ui_page_customer_info.page;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_idle_update_qr_code_prompt(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_customer_info.qr_code_prompt, "qr_code_prompt");
}

void ui_after_create_customer_info_qr_code_prompt(const char * name, lv_obj_t * obj)
{
    ui_page_customer_info.qr_code_prompt = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_idle_update_qr_code_prompt;
}

void ui_idle_update_charger_station(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_customer_info.charger_station, "charger_station");
}

void ui_after_create_customer_info_charger_station(const char * name, lv_obj_t * obj)
{
    ui_page_customer_info.charger_station = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_idle_update_charger_station;
}

void ui_after_create_customer_info(const char * name, lv_obj_t * obj)
{
    const char * idle_name = name + strlen(UI_NAME_CUSTOMER_INFO);
    if (strcmp(idle_name, UI_NAME_CUSTOMER_INFO_QR_CODE_PROMPT) == 0)
    {
        ui_after_create_customer_info_qr_code_prompt(name, obj);
        return;
    }
    else if (strcmp(idle_name, UI_NAME_CUSTOMER_INFO_CHARGER_STATION) == 0)
    {
        ui_after_create_customer_info_charger_station(name, obj);
        return;
    }
}
