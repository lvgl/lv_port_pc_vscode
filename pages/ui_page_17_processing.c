#include "../pages_mngr.h"

#define UI_NAME_17_PROCESSING_LABEL                        ".processing.charging_process_title"
#define UI_NAME_17_PROCESSING_PROMPT                       ".processing.charging_process_subtitle"
#define UI_NAME_17_PROCESSING_GIF                          ".processing.charging_process_gif"

typedef struct {

    lv_obj_t * page;

    lv_obj_t * label;
    lv_obj_t * prompt;
    lv_obj_t * gif;

} ui_page_17_processing_t;

static ui_page_17_processing_t ui_page_17_processing = {NULL};

static bool gif_playing = false;


void ui_page_show_17_processing(void)
{

    if (ui_page_17_processing.gif != NULL)
    {
        if(!gif_playing)
        {
            // LV_LOG_WARN("222=======================================");
            lv_gif_set_src(ui_page_17_processing.gif, "A:./assets/images/processing.gif"); // 因为LVGL版本原因，restart不能重启gif定时器,所以需要重置
            gif_playing = true;
        }
    }
}

void ui_page_hide_17_processing(void)
{
    if (ui_page_17_processing.gif != NULL)
    {
        if(gif_playing)
        {
            // LV_LOG_WARN("444=======================================");
            lv_gif_pause(ui_page_17_processing.gif);
            gif_playing = false;
        }
    }
}

lv_obj_t * ui_page_create_17_processing(lv_obj_t *parent)
{
    return ioc_layout_mngr_load_layout("processing", parent);
}


void ui_processing_update_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_17_processing.label, "Processing");
}

void ui_after_create_17_processing_label(const char * name, lv_obj_t * obj)
{
    ui_page_17_processing.label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_processing_update_label;
}


void ui_processing_update_prompt(struct ioc_obj_user_data_s * user_data)
{
    switch (g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX_SINGLE].startingSubStep)
    {
    case STARTING_HANDSHAKE:
        ioc_label_set_text(ui_page_17_processing.prompt, "Handshake");   // TODO 国际化
        break;
    case STARTING_INSULATION:
        ioc_label_set_text(ui_page_17_processing.prompt, "Insulation");   // TODO 国际化
        break;
    case STARTING_PARAMTETER:
        ioc_label_set_text(ui_page_17_processing.prompt, "Parameter");   // TODO 国际化
        break;
    case STARTING_PRECHARGE:
        ioc_label_set_text(ui_page_17_processing.prompt, "PreCharge");   // TODO 国际化
        break;
    case STARTING_AC:
    default:
        ioc_label_set_text(ui_page_17_processing.prompt, "... ...");
        break;
    }
}

void ui_after_create_17_processing_prompt(const char * name, lv_obj_t * obj)
{
    ui_page_17_processing.prompt = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_processing_update_prompt;
}

void ui_after_create_17_processing_gif(const char * name, lv_obj_t * obj)
{
    ui_page_17_processing.gif = obj;
}

void ui_after_create_17_processing(const char * name, lv_obj_t * obj)
{
    const char * processing_name = name + strlen(UI_NAME_17_PROCESSING);
    if (strcmp(processing_name, UI_NAME_17_PROCESSING_LABEL) == 0)
    {
        ui_after_create_17_processing_label(name, obj);
        return;
    }
    else if (strcmp(processing_name, UI_NAME_17_PROCESSING_PROMPT) == 0)
    {
        ui_after_create_17_processing_prompt(name, obj);
        return;
    }
    else if (strcmp(processing_name, UI_NAME_17_PROCESSING_GIF) == 0)
    {
        ui_after_create_17_processing_gif(name, obj);
        return;
    }
}
