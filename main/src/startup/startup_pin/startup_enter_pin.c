/*********************
 *      INCLUDES
 *********************/


#include "startup_enter_pin.h"
#include "gui_comm.h"
#include "gui_data_comm.h"


extern void settings_erase_start(void);

extern void startup_language_start(app_index_t app_index);
extern void startup_set_pin_start(app_index_t app_index);
static void startup_wrong_pin_bg_cont(lv_obj_t* parent);
static void startup_enter_pin_bg_cont(lv_obj_t* parent);


static startup_enter_pin_t* p_startup_enter_pin = NULL;

static void startup_keypad_ok_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        if (0 == lv_strcmp(p_startup_enter_pin->pin, gui_data_get_pin()))
        {
        	if(p_startup_enter_pin->app_index == APP_SETTINGS_ERASE)
        		settings_erase_start();
        	else
		        startup_set_pin_start(p_startup_enter_pin->app_index);
	        startup_enter_pin_stop();
        }
        else
        {
            startup_wrong_pin_bg_cont(p_startup_enter_pin->bg_cont);
        	lv_memset(p_startup_enter_pin->pin, 0, sizeof(p_startup_enter_pin->pin));
        }
    }
}

static void startup_keypad_cancle_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        uint8_t ter_len = lv_strlen(p_startup_enter_pin->pin);
        if (ter_len)
        {
            p_startup_enter_pin->pin[ter_len - 1] = 0;
            lv_label_set_text(p_startup_enter_pin->pin_label, p_startup_enter_pin->pin);

            if(ter_len == 1)
                lv_obj_add_flag(p_startup_enter_pin->pin_label, LV_OBJ_FLAG_HIDDEN);
        }
    }
}
static void startup_keypad_num_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_clear_flag(p_startup_enter_pin->pin_label, LV_OBJ_FLAG_HIDDEN);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        if (6 > lv_strlen(p_startup_enter_pin->pin))
        {
            lv_strcat(p_startup_enter_pin->pin, (char*)(e->user_data));
            lv_label_set_text(p_startup_enter_pin->pin_label, p_startup_enter_pin->pin);
        }
    }
}

static void startup_wrong_btn_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        startup_enter_pin_bg_cont(p_startup_enter_pin->bg_cont);
    }
}
static void startup_enter_pin_bg_cont(lv_obj_t* parent)
{
    lv_obj_clean(parent);
    gui_comm_draw_keypad_num(parent, startup_keypad_num_cb, startup_keypad_ok_cb, startup_keypad_cancle_cb);

    lv_obj_t* tip_label = lv_label_create(parent);
    lv_obj_set_style_text_color(tip_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(tip_label, &lv_font_montserrat_20, 0);
    lv_label_set_long_mode(tip_label, LV_LABEL_LONG_DOT);
    lv_obj_set_width(tip_label, 200);

    if(APP_STARTUP_CREATE_WALLET == p_startup_enter_pin->app_index || 
    	APP_STARTUP_IMPORT_WALLET == p_startup_enter_pin->app_index)
    	lv_label_set_text(tip_label, "Enter New PIN");
    else if(APP_SETTINGS_PIN == p_startup_enter_pin->app_index)
    	lv_label_set_text(tip_label, "Enter Old PIN");
    else if(APP_SETTINGS_ERASE == p_startup_enter_pin->app_index)
    	lv_label_set_text(tip_label, "Enter PIN");
    	
    lv_obj_set_style_text_align(tip_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(tip_label, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_update_layout(tip_label);

    lv_obj_t* pin_label = lv_label_create(parent);
    lv_obj_set_style_text_color(pin_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(pin_label, &lv_font_montserrat_20, 0);
    lv_label_set_long_mode(pin_label, LV_LABEL_LONG_DOT);
    lv_obj_set_width(pin_label, 200);
    lv_label_set_text(pin_label, "1");
    lv_obj_set_style_text_align(pin_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(pin_label, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_add_flag(pin_label, LV_OBJ_FLAG_HIDDEN);
    p_startup_enter_pin->pin_label = pin_label;
}

static void startup_wrong_pin_bg_cont(lv_obj_t* parent)
{
    lv_obj_clean(parent);

    lv_obj_t *label_title = lv_label_create(parent);
    lv_obj_set_style_text_color(label_title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_title, &lv_font_montserrat_20, 0);
    lv_label_set_text(label_title, "Wrong PIN");
    lv_obj_set_pos(label_title, 20, 30);

    lv_obj_t* label_describe = lv_label_create(parent);
    lv_obj_set_style_text_color(label_describe, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_describe, &lv_font_montserrat_12, 0);
    lv_label_set_text(label_describe, "The PIN you entered is incorrect.");
    lv_label_set_long_mode(label_describe, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(label_describe, 200);
    lv_obj_set_pos(label_describe, 20, 60);

    lv_obj_t* imgbtn_close = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(imgbtn_close, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_imagebutton_set_src(imgbtn_close, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_obj_set_width(imgbtn_close, 200);
    lv_obj_set_pos(imgbtn_close, 20, 278);
    lv_obj_add_event_cb(imgbtn_close, startup_wrong_btn_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(imgbtn_close, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_close = lv_label_create(imgbtn_close);
    lv_obj_set_style_text_color(label_close, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_close, &lv_font_montserrat_12, 0);
    lv_label_set_text(label_close, "Close");
    lv_obj_center(label_close);
}

void startup_enter_pin_start(app_index_t app_index)
{
    p_startup_enter_pin = (startup_enter_pin_t*)lv_malloc(sizeof(startup_enter_pin_t));
    LV_ASSERT(p_startup_enter_pin);
    lv_memset(p_startup_enter_pin, 0, sizeof(startup_enter_pin_t));

	p_startup_enter_pin->app_index = app_index;
    p_startup_enter_pin->bg_cont = gui_comm_draw_bg();
    startup_enter_pin_bg_cont(p_startup_enter_pin->bg_cont);
}

void startup_enter_pin_stop(void)
{
    lv_obj_del(p_startup_enter_pin->bg_cont);
    lv_free(p_startup_enter_pin);
    p_startup_enter_pin = NULL;
}


