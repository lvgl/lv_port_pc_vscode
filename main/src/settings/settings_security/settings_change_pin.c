/*********************
 *      INCLUDES
 *********************/

#include "settings_change_pin.h"
#include "gui_comm.h"

extern void settings_security_start(void);
extern void startup_enter_pin_start(app_index_t app_index);

static settings_change_pin_t* p_settings_change_pin = NULL;


static void confirm_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        settings_change_pin_stop();
		startup_enter_pin_start(APP_SETTINGS_PIN);
    }
}

static void cancle_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        settings_change_pin_stop();
        settings_security_start();
    }
}

static void settings_change_pin_bg_cont(lv_obj_t* parent)
{
	lv_obj_t* img_succ = lv_img_create(parent);
	lv_img_set_src(img_succ, &img_alert);
    lv_obj_set_pos(img_succ, 20, 35);

    lv_obj_t* title_label = lv_label_create(parent);
    lv_obj_set_style_text_color(title_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, 0);
    lv_label_set_long_mode(title_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(title_label, 200);
    lv_obj_set_pos(title_label, 45, 30);
    lv_label_set_text(title_label, "Change Pin");

    lv_obj_t* describe_label = lv_label_create(parent);
    lv_obj_set_style_text_color(describe_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(describe_label, &lv_font_montserrat_12, 0);
    lv_label_set_long_mode(describe_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(describe_label, 200);
    lv_obj_set_pos(describe_label, 20, 60);
    lv_label_set_text(describe_label, "Set a 4 to 50-digits length PIN to protect your wallet.");
    lv_obj_add_flag(describe_label, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t* img_btn_confirm = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_confirm, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_f7e100_14x26, &img_mid_released_f7e100_4x26, &img_right_released_f7e100_14x26);
    lv_imagebutton_set_src(img_btn_confirm, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_ffef4b_14x26, &img_mid_pressed_ffef4b_4x26, &img_right_pressed_ffef4b_14x26);
    lv_obj_set_width(img_btn_confirm, 200);
    lv_obj_set_pos(img_btn_confirm, 20, 246);
    lv_obj_add_event_cb(img_btn_confirm, confirm_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_confirm, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_confirm = lv_label_create(img_btn_confirm);
    lv_obj_set_style_text_color(label_confirm, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_font(label_confirm, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_confirm, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_confirm, LV_LABEL_LONG_DOT);
    lv_obj_set_width(label_confirm, 200);
    lv_label_set_text(label_confirm, "Confirm");
    lv_obj_center(label_confirm);
    lv_obj_add_flag(label_confirm, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t* img_btn_cancle = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_cancle, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
    lv_imagebutton_set_src(img_btn_cancle, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
    lv_obj_set_width(img_btn_cancle, 200);
    lv_obj_set_pos(img_btn_cancle, 20, 278);
    lv_obj_add_event_cb(img_btn_cancle, cancle_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_cancle, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_cancle = lv_label_create(img_btn_cancle);
    lv_obj_set_style_text_color(label_cancle, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_cancle, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_cancle, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_cancle, LV_LABEL_LONG_DOT);
    lv_obj_set_width(label_cancle, 200);
    lv_label_set_text(label_cancle, "Cancle");
    lv_obj_center(label_cancle);
    lv_obj_add_flag(label_cancle, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

}


void settings_change_pin_start(void)
{
    p_settings_change_pin = (settings_change_pin_t*)lv_malloc(sizeof(settings_change_pin_t));
    LV_ASSERT(p_settings_change_pin);
    lv_memset(p_settings_change_pin, 0, sizeof(settings_change_pin_t));

    p_settings_change_pin->bg_cont = gui_comm_draw_bg();
    settings_change_pin_bg_cont(p_settings_change_pin->bg_cont);
}

void settings_change_pin_stop(void)
{
    lv_obj_del(p_settings_change_pin->bg_cont);
    lv_free(p_settings_change_pin);
    p_settings_change_pin = NULL;
}

