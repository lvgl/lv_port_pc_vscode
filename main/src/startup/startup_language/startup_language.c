/*********************
 *      INCLUDES
 *********************/

#include "startup_language.h"
#include "gui_comm.h"
#include "gui_data_comm.h"

static gui_comm_imgbtn_desc_t startup_language_desc_table[] =
{
    {"English", 20, 66},
    {"简体中文", 20, 96},
    {"繁體中文", 20, 126},
    {"Français", 20, 156},
    {"Español", 20, 186},
    {"العربية", 20, 216},
    {"日本語", 20, 246},
    {"Русский", 20, 276},
    {"한국어", 20, 306},
    {"Português", 20, 336},
    {"Português (Brasil)", 20, 366},
    {"Italiano", 20, 396},
    {"Deutsch", 20, 426},
    {"हिन्दी", 20, 456},
    {"Монгол хэл", 20, 486},
    {"ไทย", 20, 276},
    {"Українська", 20, 516},
    {"Tiếng Việt", 20, 546},
    {"Bahasa Indonesia", 20, 576},
    {"Filipino", 20, 606},
    {"বাংলা", 20, 636},
};
extern void general_main_start(void);
extern void startup_quick_start_start(void);

static startup_language_t* p_startup_language = NULL;

static void startup_language_event_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        printf("setting language:%s\n", (char *)e->user_data);
		gui_data_set_language((char *)e->user_data);
		if(APP_GENERAL == p_startup_language->app_index)
		{
        	general_main_start();
		}
		else if(APP_STARTUP == p_startup_language->app_index)
		{
	        startup_quick_start_start();
        }
        startup_language_stop();
    }
}
static void startup_language_bg_cont(lv_obj_t* parent)
{
	gui_comm_draw_title(parent, "Language", NULL);
	    
    for (int i = 0; i < sizeof(startup_language_desc_table) / sizeof(gui_comm_imgbtn_desc_t); i++)
    {
        lv_obj_t* btn = lv_obj_create(parent);
        lv_obj_set_style_radius(btn, 15, LV_PART_MAIN);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x888888), LV_PART_MAIN);
        lv_obj_set_size(btn, 200, 26);
        lv_obj_set_pos(btn, startup_language_desc_table[i].x, startup_language_desc_table[i].y);
        lv_obj_add_event_cb(btn, startup_language_event_handler, LV_EVENT_SHORT_CLICKED, (void *)startup_language_desc_table[i].str);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLL_WITH_ARROW);
		lv_obj_remove_flag(btn, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC);

        lv_obj_t* label = lv_label_create(btn);
		lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
		lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
        lv_label_set_text(label, startup_language_desc_table[i].str);
        lv_obj_center(label);
    }
}

void startup_language_start(app_index_t app_index)
{
    p_startup_language = (startup_language_t*)lv_malloc(sizeof(startup_language_t));
    LV_ASSERT(p_startup_language);
    lv_memset(p_startup_language, 0, sizeof(startup_language_t));

	p_startup_language->app_index = app_index;
    p_startup_language->bg_cont = gui_comm_draw_bg();
	lv_obj_add_flag(p_startup_language->bg_cont, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC);
    startup_language_bg_cont(p_startup_language->bg_cont);
}

void startup_language_stop(void)
{
    lv_obj_del(p_startup_language->bg_cont);
    lv_free(p_startup_language);
    p_startup_language = NULL;
}


