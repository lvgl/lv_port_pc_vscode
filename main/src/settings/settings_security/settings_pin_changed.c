/*********************
 *      INCLUDES
 *********************/

#include "settings_pin_changed.h"
#include "gui_comm.h"

extern void settings_security_start(void);

static settings_pin_changed_t* p_settings_pin_changed = NULL;


static void done_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        settings_pin_changed_stop();
        settings_security_start();
    }
}

static void settings_pin_changed_bg_cont(lv_obj_t* parent)
{
	lv_obj_t* img_succ = lv_img_create(parent);
	lv_img_set_src(img_succ, &img_success);
    lv_obj_set_pos(img_succ, 20, 35);

    lv_obj_t* title_label = lv_label_create(parent);
    lv_obj_set_style_text_color(title_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, 0);
    lv_label_set_long_mode(title_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(title_label, 200);
    lv_obj_set_pos(title_label, 45, 30);
    lv_label_set_text(title_label, "Pin Changed");

    lv_obj_t* describe_label = lv_label_create(parent);
    lv_obj_set_style_text_color(describe_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(describe_label, &lv_font_montserrat_12, 0);
    lv_label_set_long_mode(describe_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(describe_label, 200);
    lv_obj_set_pos(describe_label, 20, 60);
    lv_label_set_text(describe_label, "You have successfully changed your PIN.");
    lv_obj_add_flag(describe_label, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t* img_btn_done = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_done, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_10e807_14x26, &img_mid_released_10e807_4x26, &img_right_released_10e807_14x26);
    lv_imagebutton_set_src(img_btn_done, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_61fc5a_14x26, &img_mid_pressed_61fc5a_4x26, &img_right_pressed_61fc5a_14x26);
    lv_obj_set_width(img_btn_done, 200);
    lv_obj_set_pos(img_btn_done, 20, 278);
    lv_obj_add_event_cb(img_btn_done, done_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_done, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_done = lv_label_create(img_btn_done);
    lv_obj_set_style_text_color(label_done, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_done, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_done, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_done, LV_LABEL_LONG_DOT);
    lv_obj_set_width(label_done, 200);
    lv_label_set_text(label_done, "Done");
    lv_obj_center(label_done);
    lv_obj_add_flag(label_done, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

}


void settings_pin_changed_start(void)
{
    p_settings_pin_changed = (settings_pin_changed_t*)lv_malloc(sizeof(settings_pin_changed_t));
    LV_ASSERT(p_settings_pin_changed);
    lv_memset(p_settings_pin_changed, 0, sizeof(settings_pin_changed_t));

    p_settings_pin_changed->bg_cont = gui_comm_draw_bg();
    settings_pin_changed_bg_cont(p_settings_pin_changed->bg_cont);
}

void settings_pin_changed_stop(void)
{
    lv_obj_del(p_settings_pin_changed->bg_cont);
    lv_free(p_settings_pin_changed);
    p_settings_pin_changed = NULL;
}

