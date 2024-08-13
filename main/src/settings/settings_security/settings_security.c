/*********************
 *      INCLUDES
 *********************/

#include "settings_security.h"
#include "gui_comm.h"

extern void settings_main_start(void);
extern void settings_reset_start(void);
extern void settings_change_pin_start(void);



static settings_security_t* p_settings_security = NULL;

static gui_comm_imgbtn_desc_t gui_comm_imgbtn_num_table[] =
{
    {"Change Password", 20, 66},
    {"Reset Device", 20, 96},
};


static void title_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        settings_security_stop();
        settings_main_start();
    }
}

static void settings_security_word_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        printf("setting guide:%s\n", (char*)e->user_data);
        settings_security_stop();
		if(0 == lv_strcmp((char*)e->user_data, "Change Password"))
		{
			settings_change_pin_start();
		}
		else if(0 == lv_strcmp((char*)e->user_data, "Reset Device"))
		{
			settings_reset_start();
		}
    }
}
static void settings_security_bg_cont(lv_obj_t* parent)
{
    gui_comm_draw_title(parent, "Security", title_cb);

    for (uint8_t i = 0; i < sizeof(gui_comm_imgbtn_num_table) / sizeof(gui_comm_imgbtn_desc_t); i++)
    {
        lv_obj_t* img_btn = lv_imagebutton_create(parent);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
        lv_obj_set_width(img_btn, 200);
        lv_obj_set_pos(img_btn, gui_comm_imgbtn_num_table[i].x, gui_comm_imgbtn_num_table[i].y);
        lv_obj_add_event_cb(img_btn, settings_security_word_handler, LV_EVENT_SHORT_CLICKED, (void *)gui_comm_imgbtn_num_table[i].str);
        lv_obj_add_flag(img_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t* label = lv_label_create(img_btn);
		lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
		lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
        lv_label_set_text(label, gui_comm_imgbtn_num_table[i].str);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    }
}


void settings_security_start(void)
{
    p_settings_security = (settings_security_t*)lv_malloc(sizeof(settings_security_t));
    LV_ASSERT(p_settings_security);
    lv_memset(p_settings_security, 0, sizeof(settings_security_t));

    p_settings_security->bg_cont = gui_comm_draw_bg();
    settings_security_bg_cont(p_settings_security->bg_cont);
}

void settings_security_stop(void)
{
    lv_obj_del(p_settings_security->bg_cont);
    lv_free(p_settings_security);
    p_settings_security = NULL;
}

