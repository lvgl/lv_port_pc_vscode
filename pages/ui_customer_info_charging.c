#include "../pages_mngr.h"

#define UI_NAME_CUSTOMER_INFO_CHARGING_QR_CODE_PROMPT      "qr_code_prompt"
#define UI_NAME_CUSTOMER_INFO_CHARGING_BTN_NOPRICE         "btn"
#define UI_NAME_CUSTOMER_INFO_CHARGING_BTN_LABEL_NOPRICE   "btn.label"

typedef struct {

    lv_obj_t * page;

    lv_obj_t * qr_code_prompt;
    lv_obj_t * btn_noprice;
    lv_obj_t * btn_label_noprice;
} ui_page_customer_info_charging_t;

static ui_page_customer_info_charging_t s_ui_page_customer_info_charging = {NULL};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void ui_page_customer_info_charging_updatejson(struct ioc_obj_user_data_s * user_data)
{
    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        return;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
lv_obj_t * ui_page_create_customer_info_charging(lv_obj_t *parent)
{

    s_ui_page_customer_info_charging.page = ioc_layout_mngr_load_layout("cuestomer_info_charging", parent);
    if (s_ui_page_customer_info_charging.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(s_ui_page_customer_info_charging.page);
        user_data->ui_json_update_cb = ui_page_customer_info_charging_updatejson;
    }

    return s_ui_page_customer_info_charging.page;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

static void charging_detailed_data_noprice_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        ui_popup_show_charging_detail(0,0);
    }
}

void ui_charging_update_qr_code_prompt(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(s_ui_page_customer_info_charging.qr_code_prompt, "qr_code_prompt");
}

void ui_after_create_customer_info_charging_qr_code_prompt(const char * name, lv_obj_t * obj)
{
    s_ui_page_customer_info_charging.qr_code_prompt = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_charging_update_qr_code_prompt;
}

void ui_after_create_customer_info_charging_btn_noprice(const char * name, lv_obj_t * obj)
{
    s_ui_page_customer_info_charging.btn_noprice = obj;
    lv_obj_add_event_cb(obj, charging_detailed_data_noprice_event_cb, LV_EVENT_CLICKED, NULL);
}

void ui_charging_update_btn_label_noprice_noprice(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(s_ui_page_customer_info_charging.btn_label_noprice, "Detailed_data");
}

void ui_after_create_customer_info_charging_btn_label_noprice(const char * name, lv_obj_t * obj)
{
    s_ui_page_customer_info_charging.btn_label_noprice = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_charging_update_btn_label_noprice_noprice;
}

void ui_after_create_customer_info_charging(const char * name, lv_obj_t * obj)
{
    const char * charging_name = name + strlen(UI_NAME_CUSTOMER_INFO_CHARGING);
    if (strcmp(charging_name, UI_NAME_CUSTOMER_INFO_CHARGING_QR_CODE_PROMPT) == 0)
    {
        ui_after_create_customer_info_charging_qr_code_prompt(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_CUSTOMER_INFO_CHARGING_BTN_NOPRICE) == 0)
    {
        ui_after_create_customer_info_charging_btn_noprice(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_CUSTOMER_INFO_CHARGING_BTN_LABEL_NOPRICE) == 0)
    {
        ui_after_create_customer_info_charging_btn_label_noprice(name, obj);
        return;
    }
    
    return;
}
