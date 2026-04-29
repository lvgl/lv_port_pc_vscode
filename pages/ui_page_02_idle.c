#include "../pages_mngr.h"


#define UI_NAME_02_IDLE_PLUG_LABEL                       ".plug_label_idle"
#define UI_NAME_02_IDLE_PLUG_PROMPT                      ".plug_prompt_idle"

typedef struct {

    lv_obj_t * page;

    lv_obj_t * plug_label;
    lv_obj_t * plug_prompt;

} ui_page_02_idle_t;

static ui_page_02_idle_t ui_page_02_idle = {NULL};


lv_obj_t * ui_page_create_02_idle(lv_obj_t *parent)
{
    memset(&ui_page_02_idle, 0, sizeof(ui_page_02_idle));

    ui_page_02_idle.page = ioc_layout_mngr_load_layout("idle", parent);
    if (ui_page_02_idle.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(ui_page_02_idle.page);
    }

    return ui_page_02_idle.page;
}

void ui_idle_update_plug_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_02_idle.plug_label, "Plug_in_frist");
}

void ui_after_create_02_idle_plug_label(const char * name, lv_obj_t * obj)
{
    ui_page_02_idle.plug_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_idle_update_plug_label;
}

void ui_idle_update_plug_prompt(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_02_idle.plug_prompt, "home_prompt");
}

void ui_after_create_02_idle_plug_prompt(const char * name, lv_obj_t * obj)
{
    ui_page_02_idle.plug_prompt = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_idle_update_plug_prompt;
}



void ui_after_create_02_idle(const char * name, lv_obj_t * obj)
{
    const char * idle_name = name + strlen(UI_NAME_02_IDLE);
    if (strcmp(idle_name, UI_NAME_02_IDLE_PLUG_LABEL) == 0)
    {
        ui_after_create_02_idle_plug_label(name, obj);
        return;
    }
    else if (strcmp(idle_name, UI_NAME_02_IDLE_PLUG_PROMPT) == 0)
    {
        ui_after_create_02_idle_plug_prompt(name, obj);
        return;
    }
}
