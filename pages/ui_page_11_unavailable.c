#include "../pages_mngr.h"

#define UI_NAME_11_UNAVAILABLE_OUT_SERVICE                  ".unavailable_title"
#define UI_NAME_11_UNAVAILABLE_PROMPT                       ".unavailable_subtitle"

typedef struct {

    lv_obj_t * page;

    lv_obj_t * out_of_service;
    lv_obj_t * prompt;

} ui_page_11_unavailable_t;

static ui_page_11_unavailable_t ui_page_11_unavailable = {NULL};


lv_obj_t * ui_page_create_11_unavailable(lv_obj_t *parent)
{
    return ioc_layout_mngr_load_layout("unavailable", parent);
}


void ui_unavailable_update_out_of_service(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_11_unavailable.out_of_service, "Out_of_service");
}

void ui_after_create_11_unavailable_out_of_service(const char * name, lv_obj_t * obj)
{
    ui_page_11_unavailable.out_of_service = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_unavailable_update_out_of_service;
}

void ui_unavailable_update_prompt(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_11_unavailable.prompt, "unavailable_prompt");
}

void ui_after_create_11_unavailable_prompt(const char * name, lv_obj_t * obj)
{
    ui_page_11_unavailable.prompt = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_unavailable_update_prompt;
}


void ui_after_create_11_unavailable(const char * name, lv_obj_t * obj)
{
    const char * unavailable_name = name + strlen(UI_NAME_11_UNAVAILABLE);
    if (strcmp(unavailable_name, UI_NAME_11_UNAVAILABLE_OUT_SERVICE) == 0)
    {
        ui_after_create_11_unavailable_out_of_service(name, obj);
        return;
    }
    else if (strcmp(unavailable_name, UI_NAME_11_UNAVAILABLE_PROMPT) == 0)
    {
        ui_after_create_11_unavailable_prompt(name, obj);
        return;
    }
}
