#include "../pages_mngr.h"

#define UI_NAME_08_SESSION_ERROR_ERROR_LABEL                 ".session_error.charging_error_title"
#define UI_NAME_08_SESSION_ERROR_ERROR_CODE                  ".session_error.charging_error_code"
#define UI_NAME_08_SESSION_ERROR_ERROR_CODE_LABEL            ".session_error.charging_error_code.label"
#define UI_NAME_08_SESSION_ERROR_ERROR_CODE_VALUE            ".session_error.charging_error_code.value"
#define UI_NAME_08_SESSION_ERROR_ERROR_PROMPT                ".session_error.charging_error_subtitle"
#define UI_NAME_08_SESSION_ERROR_CONTINUE_BTN                ".session_error.charging_error_continue_btn"
#define UI_NAME_08_SESSION_ERROR_CONTINUE_LABEL              ".session_error.charging_error_continue_btn.continue_label"

typedef struct {

    lv_obj_t * page;

    lv_obj_t * error_label;
    lv_obj_t * error_code;
    lv_obj_t * error_code_label;
    lv_obj_t * error_code_value;
    lv_obj_t * error_prompt;
    lv_obj_t * continue_btn;
    lv_obj_t * continue_label;

} ui_page_08_session_error_t;

static ui_page_08_session_error_t ui_page_08_session_error = {NULL};

lv_obj_t * ui_page_create_08_session_error(lv_obj_t *parent)
{
    return ioc_layout_mngr_load_layout("session_error", parent);
}

void ui_session_error_update_error_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_08_session_error.error_label, "Charging_session_error");
}

void ui_after_create_08_session_error_error_label(const char * name, lv_obj_t * obj)
{
    ui_page_08_session_error.error_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_session_error_update_error_label;
}

void ui_after_create_08_session_error_error_code(const char * name, lv_obj_t * obj)
{
    ui_page_08_session_error.error_code = obj;
    lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(obj, 0, 0);
}

void ui_session_error_update_error_code_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_08_session_error.error_code_label, "Error_code_session_error");
}

void ui_session_error_update_error_code_value(struct ioc_obj_user_data_s * user_data)
{
    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "%d", g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].stopCode);
    ioc_label_set_text(ui_page_08_session_error.error_code_value, buf);
}

void ui_after_create_08_session_error_error_code_label(const char * name, lv_obj_t * obj)
{
    ui_page_08_session_error.error_code_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_session_error_update_error_code_label;
}

void ui_after_create_08_session_error_error_code_value(const char * name, lv_obj_t * obj)
{
    ui_page_08_session_error.error_code_value = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_session_error_update_error_code_value;
}

void ui_session_error_update_error_prompt(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_08_session_error.error_prompt, "session_error_prompt");
}

void ui_after_create_08_session_error_error_prompt(const char * name, lv_obj_t * obj)
{
    ui_page_08_session_error.error_prompt = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_session_error_update_error_prompt;
}

static void continue_btn_event_cb(lv_event_t *e)
{

    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        g_ui_json.isUiJsonControlLocked[ UI_CONNECTOR_ACTIVE_IDX ] = false;
        ioc_ui_json_set_conn_state(UI_CONNECTOR_ACTIVE_IDX, UI_PAGE_FINISHED);
        ioc_ui_overtime_changed_page();
    }
}

void ui_after_create_08_session_error_continue_btn(const char * name, lv_obj_t * obj)
{
    ui_page_08_session_error.continue_btn = obj;
    lv_obj_add_event_cb(obj, continue_btn_event_cb, LV_EVENT_CLICKED, NULL);
}

void ui_session_error_update_continue_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_08_session_error.continue_label, "Continue");
}

void ui_after_create_08_session_error_continue_label(const char * name, lv_obj_t * obj)
{
    ui_page_08_session_error.continue_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_session_error_update_continue_label;
}

void ui_after_create_08_session_error(const char * name, lv_obj_t * obj)
{
    const char * session_error_name = name + strlen(UI_NAME_08_SESSION_ERROR);
    if (strcmp(session_error_name, UI_NAME_08_SESSION_ERROR_ERROR_LABEL) == 0)
    {
        ui_after_create_08_session_error_error_label(name, obj);
        return;
    }
    else if (strcmp(session_error_name, UI_NAME_08_SESSION_ERROR_ERROR_CODE) == 0)
    {
        ui_after_create_08_session_error_error_code(name, obj);
        return;
    }
    else if (strcmp(session_error_name, UI_NAME_08_SESSION_ERROR_ERROR_CODE_LABEL) == 0)
    {
        ui_after_create_08_session_error_error_code_label(name, obj);
        return;
    }
    else if (strcmp(session_error_name, UI_NAME_08_SESSION_ERROR_ERROR_CODE_VALUE) == 0)
    {
        ui_after_create_08_session_error_error_code_value(name, obj);
        return;
    }
    else if (strcmp(session_error_name, UI_NAME_08_SESSION_ERROR_ERROR_PROMPT) == 0)
    {
        ui_after_create_08_session_error_error_prompt(name, obj);
        return;
    }
    else if (strcmp(session_error_name, UI_NAME_08_SESSION_ERROR_CONTINUE_BTN) == 0)
    {
        ui_after_create_08_session_error_continue_btn(name, obj);
        return;
    }
    else if (strcmp(session_error_name, UI_NAME_08_SESSION_ERROR_CONTINUE_LABEL) == 0)
    {
        ui_after_create_08_session_error_continue_label(name, obj);
        return;
    }
}
