#include "../pages_mngr.h"

#define UI_NAME_STATUS_BAR_TIME              ".time"
#define UI_NAME_STATUS_BAR_HELP              ".help"
#define UI_NAME_STATUS_BAR_HELP_TEXT         ".help.text"
#define UI_NAME_STATUS_BAR_LANGUAGE          ".language"
#define UI_NAME_STATUS_BAR_LANGUAGE_ICON     ".language.icon"
#define UI_NAME_STATUS_BAR_LANGUAGE_LABEL    ".language.label"



ui_status_bar_t ui_status_bar;

lv_obj_t * ui_page_create_status_bar(lv_obj_t *parent)
{
    return ioc_layout_mngr_load_layout("status_bar", parent);
}

static void ui_status_bar_help_btn_event_cb(lv_event_t *e)
{
    ui_popup_show_help();   // 点击弹出帮助页面, 关闭由help自己控制
}
void ui_after_create_status_bar_help(const char * name, lv_obj_t * obj)
{
    ui_status_bar.help_btn = obj;
    lv_obj_add_event_cb(obj, ui_status_bar_help_btn_event_cb, LV_EVENT_CLICKED, NULL);
}


void ui_status_bar_update_help_text(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_status_bar.help_text, "Help");
}

void ui_status_bar_update_time(struct ioc_obj_user_data_s * user_data)
{
    uint8_t tz_val = g_ui_json.timeZone;
    time_t UTC = time(NULL);
    time_t shift_UTC = UTC + ((tz_val - 48) * 15 * 60);

    struct tm* tp = gmtime(&shift_UTC);

    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "%02d : %02d", tp->tm_hour, tp->tm_min);
    ioc_label_set_text(ui_status_bar.time, buf);

}

void ui_after_create_status_bar_help_text(const char * name, lv_obj_t * obj)
{
    ui_status_bar.help_text = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_status_bar_update_help_text;
}

static void ui_status_bar_language_btn_event_cb(lv_event_t *e)
{
    ui_popup_show_language();   // 点击弹出语言选择页面, 关闭由language自己控制
}
void ui_after_create_status_bar_language(const char * name, lv_obj_t * obj)
{
    ui_status_bar.lang_btn = obj;
    lv_obj_add_event_cb(obj, ui_status_bar_language_btn_event_cb, LV_EVENT_CLICKED, NULL);
}

void ui_after_create_status_bar_language_icon(const char * name, lv_obj_t * obj)
{
    ui_status_bar.lang_icon = obj;
}

void ui_after_create_status_bar_language_label(const char * name, lv_obj_t * obj)
{
    ui_status_bar.lang_label = obj;
}

void ui_after_create_status_bar_time(const char * name, lv_obj_t * obj)
{
    ui_status_bar.time = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_status_bar_update_time;
}



void ui_after_create_status_bar(const char * name, lv_obj_t * obj)
{
    const char * status_bar_name = name + strlen(UI_NAME_STATUS_BAR);
    if (strcmp(status_bar_name, UI_NAME_STATUS_BAR_HELP) == 0)
    {
        ui_after_create_status_bar_help(name, obj);
        return;
    }
    else if (strcmp(status_bar_name, UI_NAME_STATUS_BAR_HELP_TEXT) == 0)
    {
        ui_after_create_status_bar_help_text(name, obj);
        return;
    }
    else if (strcmp(status_bar_name, UI_NAME_STATUS_BAR_LANGUAGE) == 0)
    {
        ui_after_create_status_bar_language(name, obj);
        return;
    }
    else if (strcmp(status_bar_name, UI_NAME_STATUS_BAR_LANGUAGE_ICON) == 0)
    {
        ui_after_create_status_bar_language_icon(name, obj);
        return;
    }
    else if (strcmp(status_bar_name, UI_NAME_STATUS_BAR_LANGUAGE_LABEL) == 0)
    {
        ui_after_create_status_bar_language_label(name, obj);
        return;
    }
    else if (strcmp(status_bar_name, UI_NAME_STATUS_BAR_TIME) == 0)
    {
        ui_after_create_status_bar_time(name, obj);
        return;
    }
}
