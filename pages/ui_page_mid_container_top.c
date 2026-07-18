#include "../pages_mngr.h"


#define UI_NAME_MID_CONTAINER_TOP_TYPE_POWER         ".conn_type_power"
#define UI_NAME_MID_CONTAINER_TOP_STATUS             ".conn_state"
#define UI_NAME_MID_CONTAINER_TOP_CONNID_NO          ".station_id"
#define UI_NAME_MID_CONTAINER_TOP_NO                 ".conn_id"
#define UI_NAME_MID_CONTAINER_TOP_NO_TEXT            ".conn_id.conn_id_text"
#define UI_NAME_MID_CONTAINER_TOP_NO_TYPE_ICON       ".conn_id.conn_type_icon"

typedef struct {

    lv_obj_t * page;

    lv_obj_t * type_power;
    lv_obj_t * no;
    lv_obj_t * no_text;
    lv_obj_t * no_type_icon;
    lv_obj_t * status;
    lv_obj_t * connid_no;

} ui_page_mid_container_top_t;

static ui_page_mid_container_top_t ui_page_mid_container_top = {NULL};

lv_obj_t * ui_page_create_mid_container_top(lv_obj_t *parent)
{
    return ioc_layout_mngr_load_layout("conn_top_info", parent);
}

void ui_mid_container_top_update_type_power(struct ioc_obj_user_data_s * user_data)
{

    float power_kw = UI_CONNECTOR_RATED_POWER(UI_CONNECTOR_ACTIVE_IDX) / 10.0f;

    char result[32] = {0};
    snprintf(result, sizeof(result), "%s-%.1fkW", UI_CONNECTOR_CONN_TYPE_NAME(UI_CONNECTOR_ACTIVE_IDX), power_kw);

    ioc_label_set_text(ui_page_mid_container_top.type_power, result);
}

void ui_mid_container_top_update_status(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_mid_container_top.status,UI_CONNECTOR_STATE_NAME(UI_CONNECTOR_ACTIVE_IDX));

    lv_style_t * style = NULL;
    switch (UI_CONNECTOR_STATE(UI_CONNECTOR_ACTIVE_IDX))
    {
    case UI_CONNECTOR_STATE_CONNECTING:
    case UI_CONNECTOR_STATE_STARTING1:
    case UI_CONNECTOR_STATE_STARTING2:
    case UI_CONNECTOR_STATE_CHARGING:
    case UI_CONNECTOR_STATE_FINISHED:
    case UI_CONNECTOR_STATE_WAIT_IN_LINE:
    case UI_CONNECTOR_STATE_STOPPING:
    case UI_CONNECTOR_STATE_PROCESSING:
        style = ioc_style_mngr_get_style("style-starting1-status");
        break;

    case UI_CONNECTOR_STATE_RESERVED:
        style = ioc_style_mngr_get_style("style-reserved-status");
        break;

    case UI_CONNECTOR_STATE_SESSION_ERROR:
    case UI_CONNECTOR_STATE_START_ERROR:
    case UI_CONNECTOR_STATE_FAULT:
        style = ioc_style_mngr_get_style("style-session_error-status");
        break;

    case UI_CONNECTOR_STATE_UNAVAILABLE:
        style = ioc_style_mngr_get_style("style-unavaliable-status");
        break;

    case UI_CONNECTOR_STATE_SUSPEND:
        style = ioc_style_mngr_get_style("style-suspend-status");
        break;

    case UI_CONNECTOR_STATE_HOME:
    case UI_CONNECTOR_STATE_IDLE:
    default:
        style = ioc_style_mngr_get_style("style-idle-status");
        break;
    }
    lv_obj_remove_style(ui_page_mid_container_top.status, style, 0);
    lv_obj_add_style(ui_page_mid_container_top.status, style, LV_STATE_DEFAULT);
}void ui_mid_container_top_update_connid_no(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_mid_container_top.connid_no, g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].connCode);
}

void ui_mid_container_top_update_no(struct ioc_obj_user_data_s * user_data)
{
    lv_color_t bg_color;
    switch (UI_CONNECTOR_STATE(UI_CONNECTOR_ACTIVE_IDX))
    {
    case UI_CONNECTOR_STATE_CONNECTING:
    case UI_CONNECTOR_STATE_STARTING1:
    case UI_CONNECTOR_STATE_STARTING2:
    case UI_CONNECTOR_STATE_CHARGING:
    case UI_CONNECTOR_STATE_FINISHED:
    case UI_CONNECTOR_STATE_WAIT_IN_LINE:
    case UI_CONNECTOR_STATE_STOPPING:
    case UI_CONNECTOR_STATE_PROCESSING:
        bg_color = ioc_theme_getcolor("state_charging");
        break;

    case UI_CONNECTOR_STATE_RESERVED:
        bg_color = ioc_theme_getcolor("state_reserve");
        break;

    case UI_CONNECTOR_STATE_SESSION_ERROR:
    case UI_CONNECTOR_STATE_START_ERROR:
    case UI_CONNECTOR_STATE_FAULT:
        bg_color = ioc_theme_getcolor("state_fault");
        break;

    case UI_CONNECTOR_STATE_UNAVAILABLE:
        bg_color = ioc_theme_getcolor("state_unavaliable");
        break;

    case UI_CONNECTOR_STATE_SUSPEND:
        bg_color = ioc_theme_getcolor("state_warn_suspend");
        break;

    case UI_CONNECTOR_STATE_HOME:
    case UI_CONNECTOR_STATE_IDLE:
    default:
        bg_color = ioc_theme_getcolor("state_idle_success");
        break;
    }
    lv_obj_set_style_bg_color(ui_page_mid_container_top.no, bg_color, LV_STATE_DEFAULT);
}

void ui_mid_container_top_update_no_text(struct ioc_obj_user_data_s * user_data)
{
    char str[5];
    sprintf(str, "%d", UI_CONNECTOR_ACTIVE_IDX+1);
    ioc_label_set_text(ui_page_mid_container_top.no_text, str);
}

void ui_mid_container_top_update_no_type_icon(struct ioc_obj_user_data_s * user_data)
{
    lv_img_set_src(ui_page_mid_container_top.no_type_icon, ui_conn_type_img(UI_CONNECTOR_CONN_TYPE(UI_CONNECTOR_ACTIVE_IDX)));
}


void ui_after_create_mid_container_top_type_power(const char * name, lv_obj_t * obj)
{
    ui_page_mid_container_top.type_power = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_mid_container_top_update_type_power;
}

void ui_after_create_mid_container_top_status(const char * name, lv_obj_t * obj)
{
    ui_page_mid_container_top.status = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_mid_container_top_update_status;
}
void ui_after_create_mid_container_top_connid_no(const char * name, lv_obj_t * obj)
{
    ui_page_mid_container_top.connid_no = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_mid_container_top_update_connid_no;
}

void ui_after_create_mid_container_top_no(const char * name, lv_obj_t * obj)
{
    ui_page_mid_container_top.no = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_mid_container_top_update_no;
}

void ui_after_create_mid_container_top_no_text(const char * name, lv_obj_t * obj)
{
    ui_page_mid_container_top.no_text = obj;
    if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE ) {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_mid_container_top_update_no_text;
}

void ui_after_create_mid_container_top_no_type_icon(const char * name, lv_obj_t * obj)
{
    ui_page_mid_container_top.no_type_icon = obj;
    if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE ) {
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
        user_data->ui_json_update_cb = ui_mid_container_top_update_no_type_icon;
    }
}

void ui_after_create_state_mid_container_top(const char * name, lv_obj_t * obj)
{
    const char * mid_container_top_name = name + strlen(UI_NAME_STATE_MID_CONTAINER_TOP);
    if (strcmp(mid_container_top_name, UI_NAME_MID_CONTAINER_TOP_TYPE_POWER) == 0)
    {
        ui_after_create_mid_container_top_type_power(name, obj);
        return;
    }
    else if (strcmp(mid_container_top_name, UI_NAME_MID_CONTAINER_TOP_STATUS) == 0)
    {
        ui_after_create_mid_container_top_status(name, obj);
        return;
    }
    else if (strcmp(mid_container_top_name, UI_NAME_MID_CONTAINER_TOP_CONNID_NO) == 0)
    {
        ui_after_create_mid_container_top_connid_no(name, obj);
        return;
    }
    else if (strcmp(mid_container_top_name, UI_NAME_MID_CONTAINER_TOP_NO) == 0)
    {
        ui_after_create_mid_container_top_no(name, obj);
        return;
    }
    else if (strcmp(mid_container_top_name, UI_NAME_MID_CONTAINER_TOP_NO_TEXT) == 0)
    {
        ui_after_create_mid_container_top_no_text(name, obj);
        return;
    }
    else if (strcmp(mid_container_top_name, UI_NAME_MID_CONTAINER_TOP_NO_TYPE_ICON) == 0)
    {
        ui_after_create_mid_container_top_no_type_icon(name, obj);
        return;
    }

}
