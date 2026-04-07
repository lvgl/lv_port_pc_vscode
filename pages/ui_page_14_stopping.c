#include "../pages_mngr.h"

#define UI_NAME_14_STOPPING_LABEL                       ".stopping_title"

typedef struct {

    lv_obj_t * page;

    lv_obj_t * stopping_label;

} ui_page_14_stopping_t;

static ui_page_14_stopping_t ui_page_14_stopping = {NULL};


lv_obj_t * ui_page_create_14_stopping(lv_obj_t *parent)
{
    return ioc_layout_mngr_load_layout("stopping", parent);
}

void ui_stopping_update_stopping_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_14_stopping.stopping_label, "Charging_is_stopping");
}

void ui_after_create_14_stopping_stopping_label(const char * name, lv_obj_t * obj)
{
    ui_page_14_stopping.stopping_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_stopping_update_stopping_label;
}

void ui_after_create_14_stopping(const char * name, lv_obj_t * obj)
{
    const char * stopping_name = name + strlen(UI_NAME_14_STOPPING);
    if (strcmp(stopping_name, UI_NAME_14_STOPPING_LABEL) == 0)
    {
        ui_after_create_14_stopping_stopping_label(name, obj);
        return;
    }
}
