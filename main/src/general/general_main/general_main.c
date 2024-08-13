/*********************
 *      INCLUDES
 *********************/

#include "general_main.h"
#include "gui_comm.h"
#include "gui_data_comm.h"


extern void menu_main_start(void);
extern void general_lock_start(void);
extern void general_shutdown_start(void);
extern void general_homescreen_start(void);
extern void startup_language_start(app_index_t app_index);



static general_main_t* p_general_main = NULL;

static gui_comm_imgbtn_desc_t gui_comm_imgbtn_num_table[] =
{
    {"Auto-Lock", 20, 66},
    {"Language", 20, 96},
    {"Shutdown", 20, 126},
    {"Homescreen", 20, 156},
    {"Lock Screen", 20, 186},
};

static void title_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        general_main_stop();
        menu_main_start();
    }
}

static void general_main_word_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        printf("setting general:%s\n", (char*)e->user_data);
        
		general_main_stop();
		if(0 == lv_strcmp((char*)e->user_data, "Auto-Lock"))
		{
			general_lock_start();
		}
		else if(0 == lv_strcmp((char*)e->user_data, "Language"))
		{
			startup_language_start(APP_GENERAL);
		}
		else if(0 == lv_strcmp((char*)e->user_data, "Shutdown"))
		{
			general_shutdown_start();
		}
		else if(0 == lv_strcmp((char*)e->user_data, "Homescreen"))
		{
			general_homescreen_start();
		}
		else if(0 == lv_strcmp((char*)e->user_data, "Lock Screen"))
		{
		}
    }
}

static void general_main_bg_cont(lv_obj_t* parent)
{
    gui_comm_draw_title(parent, "General", title_cb);

    for (uint8_t i = 0; i < sizeof(gui_comm_imgbtn_num_table) / sizeof(gui_comm_imgbtn_desc_t); i++)
    {
        lv_obj_t* img_btn = lv_imagebutton_create(parent);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
        lv_obj_set_width(img_btn, 200);
        lv_obj_set_pos(img_btn, gui_comm_imgbtn_num_table[i].x, gui_comm_imgbtn_num_table[i].y);
        lv_obj_add_event_cb(img_btn, general_main_word_handler, LV_EVENT_SHORT_CLICKED, (void *)gui_comm_imgbtn_num_table[i].str);
        lv_obj_add_flag(img_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t* label = lv_label_create(img_btn);
		lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
		lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
        lv_label_set_text(label, gui_comm_imgbtn_num_table[i].str);
        lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, 2);

        if (0 == i)
        {
            lv_obj_t* labe_detail = lv_label_create(img_btn);
			lv_obj_set_style_text_color(labe_detail, lv_color_hex(0xffffff), 0);
			lv_obj_set_style_text_font(labe_detail, &lv_font_montserrat_12, 0);
            lv_label_set_text_fmt(labe_detail, "%d minutes", gui_data_get_lock_time());
            lv_obj_align(labe_detail, LV_ALIGN_RIGHT_MID, -17, 2);
        }
        else if (1 == i)
        {
            lv_obj_t* labe_detail = lv_label_create(img_btn);
			lv_obj_set_style_text_color(labe_detail, lv_color_hex(0xffffff), 0);
			lv_obj_set_style_text_font(labe_detail, &lv_font_montserrat_12, 0);
            lv_label_set_text_fmt(labe_detail, "%s", gui_data_get_language());
            lv_obj_align(labe_detail, LV_ALIGN_RIGHT_MID, -17, 2);
        }
        else if (2 == i)
        {
            lv_obj_t* labe_detail = lv_label_create(img_btn);
			lv_obj_set_style_text_color(labe_detail, lv_color_hex(0xffffff), 0);
			lv_obj_set_style_text_font(labe_detail, &lv_font_montserrat_12, 0);
            lv_label_set_text_fmt(labe_detail, "%d minutes", gui_data_get_shutdown_time());
            lv_obj_align(labe_detail, LV_ALIGN_RIGHT_MID, -17, 2);
        }
        else
            lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    }
}


void general_main_start(void)
{
    p_general_main = (general_main_t*)lv_malloc(sizeof(general_main_t));
    LV_ASSERT(p_general_main);
    lv_memset(p_general_main, 0, sizeof(general_main_t));

    p_general_main->bg_cont = gui_comm_draw_bg();
    general_main_bg_cont(p_general_main->bg_cont);
}

void general_main_stop(void)
{
    lv_obj_del(p_general_main->bg_cont);
    lv_free(p_general_main);
    p_general_main = NULL;
}


