#include "../pages_mngr.h"


#define UI_NAME_01_HOME_PLUG_LABEL                       ".home_plug_title"
#define UI_NAME_01_HOME_PLUG_PROMPT                      ".home_plug_subtitle"


typedef struct {

    lv_obj_t * page;

    lv_obj_t * plug_label;
    lv_obj_t * plug_prompt;

} ui_page_01_home_t;

static ui_page_01_home_t ui_page_01_home = {NULL};

lv_obj_t * ui_page_create_01_home(lv_obj_t *parent)
{
    return ioc_layout_mngr_load_layout("home", parent);
}

void ui_home_update_plug_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_01_home.plug_label, "Plug_in_frist");
}

void ui_after_create_01_home_plug_label(const char * name, lv_obj_t * obj)
{
    ui_page_01_home.plug_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_home_update_plug_label;
}

void ui_home_update_plug_prompt(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_01_home.plug_prompt, "home_prompt");
}

void ui_after_create_01_home_plug_prompt(const char * name, lv_obj_t * obj)
{
    ui_page_01_home.plug_prompt = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_home_update_plug_prompt;
}


void ui_after_create_state_home(const char * name, lv_obj_t * obj)
{
    const char * home_name = name + strlen(UI_NAME_STATE_HOME);
    if (strcmp(home_name, UI_NAME_01_HOME_PLUG_LABEL) == 0)
    {
        ui_after_create_01_home_plug_label(name, obj);
        return;
    }
    else if (strcmp(home_name, UI_NAME_01_HOME_PLUG_PROMPT) == 0)
    {
        ui_after_create_01_home_plug_prompt(name, obj);
        return;
    }
}
