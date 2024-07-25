/*********************
 *      INCLUDES
 *********************/

#include "connect_reset.h"
#include "gui_comm.h"


static connect_reset_t* p_connect_reset = NULL;

static void connect_reset_word_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        connect_reset_stop();
    }
}
static void connect_reset_bg_cont(lv_obj_t* parent)
{
    lv_obj_t* label_title = lv_label_create(parent);
    lv_obj_set_style_text_color(label_title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_title, &lv_font_montserrat_20, 0);
    lv_label_set_text(label_title, "Connect App Wallet");
    lv_label_set_long_mode(label_title, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label_title, 220);
    lv_obj_set_pos(label_title, 20, 30);

    lv_obj_t* label_describe = lv_label_create(parent);
    lv_obj_set_style_text_color(label_describe, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_describe, &lv_font_montserrat_12, 0);
    lv_label_set_text(label_describe, "Bluetooth Pairing and enter the pairing code on your device.");
    lv_label_set_long_mode(label_describe, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label_describe, 200);
    lv_obj_set_pos(label_describe, 20, 60);

    lv_obj_t* img_btn_pair = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_pair, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_6c6cf4_16x80, &img_mid_released_6c6cf4_16x80, &img_right_released_6c6cf4_16x80);
    lv_imagebutton_set_src(img_btn_pair, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_released_6c6cf4_16x80, &img_mid_released_6c6cf4_16x80, &img_right_released_6c6cf4_16x80);
    lv_obj_set_width(img_btn_pair, 200);
    lv_obj_set_pos(img_btn_pair, 20, 120);
    lv_obj_add_event_cb(img_btn_pair, connect_reset_word_handler, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_pair, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_pair = lv_label_create(img_btn_pair);
    lv_obj_set_style_text_color(label_pair, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_pair, &lv_font_montserrat_20, 0);
    lv_label_set_text(label_pair, "123456");
    lv_obj_align(label_pair, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(label_pair, LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t* img_btn_close = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_close, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
    lv_imagebutton_set_src(img_btn_close, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
    lv_obj_set_width(img_btn_close, 200);
    lv_obj_set_pos(img_btn_close, 20, 278);
    lv_obj_add_event_cb(img_btn_close, connect_reset_word_handler, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_close, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_close = lv_label_create(img_btn_close);
    lv_obj_set_style_text_color(label_close, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_close, &lv_font_montserrat_12, 0);
    lv_label_set_text(label_close, "Close");
    lv_obj_align(label_close, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(label_close, LV_OBJ_FLAG_EVENT_BUBBLE);
}


void connect_reset_start(void)
{
    p_connect_reset = (connect_reset_t*)lv_malloc(sizeof(connect_reset_t));
    LV_ASSERT(p_connect_reset);
    lv_memset(p_connect_reset, 0, sizeof(connect_reset_t));

    p_connect_reset->bg_cont = gui_comm_draw_bg();
    connect_reset_bg_cont(p_connect_reset->bg_cont);
}

 void connect_reset_stop(void)
{
    lv_obj_del(p_connect_reset->bg_cont);
    lv_free(p_connect_reset);
    p_connect_reset = NULL;
}
