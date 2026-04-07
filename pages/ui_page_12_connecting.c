#include "../pages_mngr.h"

#define UI_NAME_12_CONNECTING_PLUG_CHARGE_LABEL                        ".connecting_title"
#define UI_NAME_12_CONNECTING_PLUG_CHARGE_PROMPT                       ".connecting_subtitle"

typedef struct {

    lv_obj_t * page;

    lv_obj_t * plug_charge_label;
    lv_obj_t * plug_charge_prompt;

} ui_page_12_connecting_t;

static ui_page_12_connecting_t ui_page_12_connecting = {NULL};


lv_obj_t * ui_page_create_12_connecting(lv_obj_t *parent)
{
    return ioc_layout_mngr_load_layout("connecting", parent);
}

void ui_connecting_update_plug_charge_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_12_connecting.plug_charge_label, "Plug & Charge");
}

void ui_after_create_12_connecting_plug_charge_label(const char * name, lv_obj_t * obj)
{
    ui_page_12_connecting.plug_charge_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_connecting_update_plug_charge_label;
}


void ui_connecting_update_plug_charge_prompt(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_12_connecting.plug_charge_prompt, "connecting_prompt");
}

void ui_after_create_12_connecting_plug_charge_prompt(const char * name, lv_obj_t * obj)
{
    ui_page_12_connecting.plug_charge_prompt = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_connecting_update_plug_charge_prompt;
}

void ui_after_create_12_connecting(const char * name, lv_obj_t * obj)
{
    const char * connecting_name = name + strlen(UI_NAME_12_CONNECTING);
    if (strcmp(connecting_name, UI_NAME_12_CONNECTING_PLUG_CHARGE_LABEL) == 0)
    {
        ui_after_create_12_connecting_plug_charge_label(name, obj);
        return;
    }
    else if (strcmp(connecting_name, UI_NAME_12_CONNECTING_PLUG_CHARGE_PROMPT) == 0)
    {
        ui_after_create_12_connecting_plug_charge_prompt(name, obj);
        return;
    }
}
