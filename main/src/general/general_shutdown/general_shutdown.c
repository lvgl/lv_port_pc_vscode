/*********************
 *      INCLUDES
 *********************/

#include "general_shutdown.h"
#include "gui_comm.h"
#include "gui_data_comm.h"

extern void general_main_start(void);

static general_shutdown_t* p_general_shutdown = NULL;

static gui_comm_imgbtn_desc_t gui_comm_imgbtn_num_table[] =
{
    {"1 minutes", 20, 66},
    {"2 minutes", 20, 96},
    {"5 minutes", 20, 126},
    {"10 minutes", 20, 156},
    {"Never", 20, 186},
};

static void title_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        general_shutdown_stop();
        general_main_start();
    }
}

static void general_shutdown_word_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        printf("shutdown time:%s\n", (char*)e->user_data);
		if(0 == lv_strcmp((char*)e->user_data, "1"))
		{
			p_general_shutdown->shutdown_time = 1;
		}
		else if(0 == lv_strcmp((char*)e->user_data, "2"))
		{
			p_general_shutdown->shutdown_time = 2;
		}
		else if(0 == lv_strcmp((char*)e->user_data, "5"))
		{
			p_general_shutdown->shutdown_time = 5;
		}
		else if(0 == lv_strcmp((char*)e->user_data, "10"))
		{
			p_general_shutdown->shutdown_time = 10;
		}
		else if(0 == lv_strcmp((char*)e->user_data, "Never"))
		{
			p_general_shutdown->shutdown_time = -1;
		}
		gui_data_set_shutdown_time(p_general_shutdown->shutdown_time);
        
		lv_imagebutton_set_src(e->original_target, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
		lv_imagebutton_set_src(e->original_target, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);

        lv_imagebutton_set_src(p_general_shutdown->choosen_btn, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
        lv_imagebutton_set_src(p_general_shutdown->choosen_btn, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);

		if(-1 == p_general_shutdown->shutdown_time)
			lv_label_set_text(p_general_shutdown->label_describe, "The device will never shut down.");
		else
			lv_label_set_text_fmt(p_general_shutdown->label_describe, "During the device locked (screen off), it will shut down after %d minutes of inactivity.", p_general_shutdown->shutdown_time);

        p_general_shutdown->choosen_btn = e->original_target;
    }
}

static void general_shutdown_bg_cont(lv_obj_t* parent)
{
    gui_comm_draw_title(parent, "Shutdown", title_cb);
    uint8_t choosen;
	if(1 == p_general_shutdown->shutdown_time)
		choosen = 0;
	else if(2 == p_general_shutdown->shutdown_time)
		choosen = 1;
	else if(5 == p_general_shutdown->shutdown_time)
		choosen = 2;
	else if(10 == p_general_shutdown->shutdown_time)
		choosen = 3;
	else if(-1 == p_general_shutdown->shutdown_time)
		choosen = 4;
	
    for (uint8_t i = 0; i < sizeof(gui_comm_imgbtn_num_table) / sizeof(gui_comm_imgbtn_desc_t); i++)
    {
        lv_obj_t* img_btn = lv_imagebutton_create(parent);
        if(choosen == i)
        {
        	p_general_shutdown->choosen_btn = img_btn;
			lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
			lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
        }
        else
        {
			lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
			lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
        }
        lv_obj_set_width(img_btn, 200);
        lv_obj_set_pos(img_btn, gui_comm_imgbtn_num_table[i].x, gui_comm_imgbtn_num_table[i].y);
        lv_obj_add_event_cb(img_btn, general_shutdown_word_handler, LV_EVENT_SHORT_CLICKED, (void *)gui_comm_imgbtn_num_table[i].str);
        lv_obj_add_flag(img_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t* label = lv_label_create(img_btn);
		lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
		lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
        lv_label_set_text(label, gui_comm_imgbtn_num_table[i].str);
        lv_obj_center(label);
    }
    lv_obj_t* label_describe = lv_label_create(parent);
    lv_obj_set_style_text_color(label_describe, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_describe, &lv_font_montserrat_12, 0);
    lv_label_set_long_mode(label_describe, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label_describe, 168);
    lv_obj_set_pos(label_describe, 20, 222);
    if(-1 == p_general_shutdown->shutdown_time)
		lv_label_set_text(label_describe, "The device will never shut down.");
    else
	    lv_label_set_text_fmt(label_describe, "During the device locked (screen off), it will shut down after %d minutes of inactivity.", p_general_shutdown->shutdown_time);
	lv_obj_update_layout(label_describe);
	p_general_shutdown->label_describe = label_describe;
}


void general_shutdown_start(void)
{
    p_general_shutdown = (general_shutdown_t*)lv_malloc(sizeof(general_shutdown_t));
    LV_ASSERT(p_general_shutdown);
    lv_memset(p_general_shutdown, 0, sizeof(general_shutdown_t));
	p_general_shutdown->shutdown_time = gui_data_get_shutdown_time();

    p_general_shutdown->bg_cont = gui_comm_draw_bg();
    general_shutdown_bg_cont(p_general_shutdown->bg_cont);
}

void general_shutdown_stop(void)
{
    lv_obj_del(p_general_shutdown->bg_cont);
    lv_free(p_general_shutdown);
    p_general_shutdown = NULL;
}


