#include "../pages_mngr.h"

#define UI_NAME_15_WAIT_IN_LINE_LABEL                    ".wait_in_line.charging_process_title"
#define UI_NAME_15_WAIT_IN_LINE_PROMPT                   ".wait_in_line.charging_process_subtitle"
#define UI_NAME_15_WAIT_IN_LINE_PROMPT_GIF               ".wait_in_line.charging_process_gif"


typedef struct {

    lv_obj_t * page;

    lv_obj_t * label;
    lv_obj_t * prompt;
    lv_obj_t * gif;

} ui_page_15_wait_in_line_t;

static ui_page_15_wait_in_line_t ui_page_15_wait_in_line = {NULL};

static bool gif_playing = false;

void ui_page_show_15_wait_in_line(void)
{
    if (ui_page_15_wait_in_line.gif != NULL)
    {
        if(!gif_playing)
        {
        // LV_LOG_WARN("666=======================================");
        lv_gif_set_src(ui_page_15_wait_in_line.gif, "A:./assets/images/processing.gif"); // 因为LVGL版本原因，restart不能重启gif定时器,所以需要重置
        gif_playing = true;
        }
    }
}

void ui_page_hide_15_wait_in_line(void)
{
    if (ui_page_15_wait_in_line.gif != NULL)
    {
        // LV_LOG_WARN("888=======================================");
        if(gif_playing)
        {
            lv_gif_pause(ui_page_15_wait_in_line.gif);
        }
    }
}


lv_obj_t * ui_page_create_15_wait_in_line(lv_obj_t *parent)
{
    return ioc_layout_mngr_load_layout("wait_in_line", parent);
}

void ui_wait_in_line_update_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_15_wait_in_line.label, "Wait_in_line");
}

void ui_after_create_15_wait_in_line_label(const char * name, lv_obj_t * obj)
{
    ui_page_15_wait_in_line.label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_wait_in_line_update_label;
}

void ui_wait_in_line_update_prompt(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_15_wait_in_line.prompt, "Wait_in_line_prompt");
}

void ui_after_create_15_wait_in_line_prompt(const char * name, lv_obj_t * obj)
{
    ui_page_15_wait_in_line.prompt = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_wait_in_line_update_prompt;
}

void ui_after_create_15_wait_in_line_gif(const char * name, lv_obj_t * obj)
{
    ui_page_15_wait_in_line.gif = obj;
}

void ui_after_create_15_wait_in_line(const char * name, lv_obj_t * obj)
{
    const char * wait_in_line_label_name = name + strlen(UI_NAME_15_WAIT_IN_LINE);
    if (strcmp(wait_in_line_label_name, UI_NAME_15_WAIT_IN_LINE_LABEL) == 0)
    {
        ui_after_create_15_wait_in_line_label(name, obj);
        return;
    }
    else if (strcmp(wait_in_line_label_name, UI_NAME_15_WAIT_IN_LINE_PROMPT) == 0)
    {
        ui_after_create_15_wait_in_line_prompt(name, obj);
        return;
    }
    else if (strcmp(wait_in_line_label_name, UI_NAME_15_WAIT_IN_LINE_PROMPT_GIF) == 0)
    {
        ui_after_create_15_wait_in_line_gif(name, obj);
        return;
    }
}
