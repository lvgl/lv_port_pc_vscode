/*********************
 *      INCLUDES
 *********************/

#include "settings_about.h"
#include "gui_comm.h"

extern void settings_main_start(void);

static settings_about_t* p_settings_about = NULL;

static void title_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        settings_about_stop();
        settings_main_start();
    }
}

static void settings_about_bg_cont(lv_obj_t* parent)
{
    gui_comm_draw_title(parent, "About Device", title_cb);

    lv_obj_t *bg_obj = lv_obj_create(parent);
	lv_obj_set_scroll_dir(bg_obj, LV_DIR_VER);
	lv_obj_set_style_bg_color(bg_obj, lv_color_hex(0x888888), LV_PART_MAIN);
    lv_obj_set_size(bg_obj, 200, 238);
    lv_obj_set_pos(bg_obj, 20, 66);

    lv_obj_t *label_model = lv_label_create(bg_obj);
    lv_obj_set_style_text_color(label_model, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_model, &lv_font_montserrat_12, 0);
    lv_obj_set_pos(label_model, 20, 20);
    lv_label_set_text(label_model, "Model");
    lv_obj_add_flag(label_model, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
    
    lv_obj_t *label_model_info = lv_label_create(bg_obj);
    lv_obj_set_style_text_color(label_model_info, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_model_info, &lv_font_montserrat_12, 0);
    lv_obj_set_pos(label_model_info, 20, 35);
    lv_label_set_text(label_model_info, "LIKKIM");
    lv_obj_add_flag(label_model_info, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
    
    lv_obj_t *label_bluetooth = lv_label_create(bg_obj);
    lv_obj_set_style_text_color(label_bluetooth, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_bluetooth, &lv_font_montserrat_12, 0);
    lv_obj_set_pos(label_bluetooth, 20, 60);
    lv_label_set_text(label_bluetooth, "Bluetooth Name");
    lv_obj_add_flag(label_bluetooth, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
	
    lv_obj_t *label_bluetooth_info = lv_label_create(bg_obj);
    lv_obj_set_style_text_color(label_bluetooth_info, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_bluetooth_info, &lv_font_montserrat_12, 0);
    lv_obj_set_pos(label_bluetooth_info, 20, 75);
    lv_label_set_text(label_bluetooth_info, "LIKKIM");
    lv_obj_add_flag(label_bluetooth_info, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
	
    lv_obj_t *label_system = lv_label_create(bg_obj);
    lv_obj_set_style_text_color(label_system, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_system, &lv_font_montserrat_12, 0);
    lv_obj_set_pos(label_system, 20, 100);
    lv_label_set_text(label_system, "system Name");
    lv_obj_add_flag(label_system, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
	
    lv_obj_t *label_system_info = lv_label_create(bg_obj);
    lv_obj_set_style_text_color(label_system_info, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_system_info, &lv_font_montserrat_12, 0);
    lv_obj_set_pos(label_system_info, 20, 115);
    lv_label_set_text(label_system_info, "Syetem");
    lv_obj_add_flag(label_system_info, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
	
    lv_obj_t *label_bootloader = lv_label_create(bg_obj);
    lv_obj_set_style_text_color(label_bootloader, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_bootloader, &lv_font_montserrat_12, 0);
    lv_obj_set_pos(label_bootloader, 20, 140);
    lv_label_set_text(label_bootloader, "bootloader Name");
    lv_obj_add_flag(label_bootloader, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
	
    lv_obj_t *label_bootloader_info = lv_label_create(bg_obj);
    lv_obj_set_style_text_color(label_bootloader_info, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_bootloader_info, &lv_font_montserrat_12, 0);
    lv_obj_set_pos(label_bootloader_info, 20, 155);
    lv_label_set_text(label_bootloader_info, "Syetem");
    lv_obj_add_flag(label_bootloader_info, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
}


void settings_about_start(void)
{
    p_settings_about = (settings_about_t*)lv_malloc(sizeof(settings_about_t));
    LV_ASSERT(p_settings_about);
    lv_memset(p_settings_about, 0, sizeof(settings_about_t));

    p_settings_about->bg_cont = gui_comm_draw_bg();
    settings_about_bg_cont(p_settings_about->bg_cont);
}

 void settings_about_stop(void)
{
    lv_obj_del(p_settings_about->bg_cont);
    lv_free(p_settings_about);
    p_settings_about = NULL;
}
