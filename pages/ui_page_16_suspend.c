#include "../pages_mngr.h"

#define UI_NAME_16_SUSPEND_LABEL                        ".suspend_label"
#define UI_NAME_16_SUSPEND_REASON                       ".suspend_reason"


typedef struct {

    lv_obj_t * page;

    lv_obj_t * suspend_label;
    lv_obj_t * suspend_reason;


} ui_page_16_suspend_t;

static ui_page_16_suspend_t ui_page_16_suspend = {NULL};

lv_obj_t * ui_page_create_16_suspend(lv_obj_t *parent)
{
    return ioc_layout_mngr_load_layout("suspend", parent);
}


void ui_suspend_update_suspend_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_16_suspend.suspend_label, "Charging_suspend");
}

void ui_after_create_16_suspend_suspend_label(const char * name, lv_obj_t * obj)
{
    ui_page_16_suspend.suspend_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_suspend_update_suspend_label;
}

void ui_suspend_update_suspend_reason(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_16_suspend.suspend_reason, "");
}

void ui_after_create_16_suspend_suspend_reason(const char * name, lv_obj_t * obj)
{
    ui_page_16_suspend.suspend_reason = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_suspend_update_suspend_reason;
}

void ui_after_create_16_suspend(const char * name, lv_obj_t * obj)
{
    const char * suspend_label_name = name + strlen(UI_NAME_16_SUSPEND);
    if (strcmp(suspend_label_name, UI_NAME_16_SUSPEND_LABEL) == 0)
    {
        ui_after_create_16_suspend_suspend_label(name, obj);
        return;
    }
    else if (strcmp(suspend_label_name, UI_NAME_16_SUSPEND_REASON) == 0)
    {
        ui_after_create_16_suspend_suspend_reason(name, obj);
        return;
    }
}
