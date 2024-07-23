/*********************
 *      INCLUDES
 *********************/

#include "guide_comm.h"


static guide_imgbtn_desc_t guide_imgbtn_num_table[] =
{
    {"1", 20, 132},
    {"2", 88, 132},
    {"3", 156, 132},
    {"4", 20, 176},
    {"5", 88, 176},
    {"6", 156, 176},
    {"7", 20, 220},
    {"8", 88, 220},
    {"9", 156, 220},
    {"0", 88, 264},
};


void guide_draw_keypad(lv_obj_t* parent, lv_event_cb_t num_cb, lv_event_cb_t ok_cb, lv_event_cb_t cancle_cb)
{
    for (uint8_t i = 0; i < sizeof(guide_imgbtn_num_table) / sizeof(guide_imgbtn_desc_t); i++)
    {
        lv_obj_t* img_btn = lv_imagebutton_create(parent);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_16x40, &img_mid_released_888888_4x40, &img_right_released_888888_16x40);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_16x40, &img_mid_pressed_bbbbbb_4x40, &img_right_pressed_bbbbbb_16x40);
        lv_obj_set_width(img_btn, 64);
        lv_obj_set_pos(img_btn, guide_imgbtn_num_table[i].x, guide_imgbtn_num_table[i].y);
        lv_obj_add_event_cb(img_btn, num_cb, LV_EVENT_SHORT_CLICKED, (void *)guide_imgbtn_num_table[i].str);
        lv_obj_add_flag(img_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t *label = lv_label_create(img_btn);
        lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
        lv_label_set_text(label, guide_imgbtn_num_table[i].str);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    }

    lv_obj_t* img_btn_ok = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_ok, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_6c6cf4_16x40, &img_mid_released_6c6cf4_4x40, &img_right_released_6c6cf4_16x40);
    lv_imagebutton_set_src(img_btn_ok, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_8f7ff0_16x40, &img_mid_pressed_8f7ff0_4x40, &img_right_pressed_8f7ff0_16x40);
    lv_obj_set_width(img_btn_ok, 64);
    lv_obj_set_pos(img_btn_ok, 156, 264);
    lv_obj_add_event_cb(img_btn_ok, ok_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_ok, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *img_ok = lv_img_create(img_btn_ok);
    lv_img_set_src(img_ok, &img_imgbtn_ok_12x9);
    lv_obj_center(img_ok);

    lv_obj_t* img_btn_cancle = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_cancle, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_ff2600_16x40, &img_mid_released_ff2600_4x40, &img_right_released_ff2600_16x40);
    lv_imagebutton_set_src(img_btn_cancle, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_ff593c_16x40, &img_mid_pressed_ff593c_4x40, &img_right_pressed_ff593c_16x40);
    lv_obj_set_width(img_btn_cancle, 64);
    lv_obj_set_pos(img_btn_cancle, 20, 264);
    lv_obj_add_event_cb(img_btn_cancle, cancle_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_cancle, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* img_canle = lv_img_create(img_btn_cancle);
    lv_img_set_src(img_canle, &img_imgbtn_cancle_15x15);
    lv_obj_center(img_canle);
}

lv_obj_t *guide_draw_bg_cont(void)
{
    lv_obj_t *bg_obj = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(bg_obj);
    lv_obj_set_style_bg_opa(bg_obj, LV_OPA_COVER, LV_PART_ANY);
    lv_obj_set_style_bg_grad_opa(bg_obj, LV_OPA_COVER, LV_PART_ANY);
    lv_obj_set_style_bg_main_opa(bg_obj, LV_OPA_COVER, LV_PART_ANY);
    
	lv_obj_set_scrollbar_mode(bg_obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_remove_flag(bg_obj, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_set_style_bg_color(bg_obj, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_size(bg_obj, 240, 320);
    lv_obj_center(bg_obj);
	lv_obj_update_layout(bg_obj);
	
    return bg_obj;
}

lv_obj_t* guide_draw_img_btn(lv_obj_t* parent, const void *released_src, const void *pressed_src)
{
    lv_obj_t* bg_obj = lv_imagebutton_create(lv_scr_act());
	lv_obj_set_scrollbar_mode(bg_obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_remove_flag(bg_obj, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_imagebutton_set_src(bg_obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, released_src, NULL);
    lv_imagebutton_set_src(bg_obj, LV_IMAGEBUTTON_STATE_PRESSED, NULL, pressed_src, NULL);

    return bg_obj;
}

lv_obj_t *guide_draw_title(lv_obj_t *parent, const char *txt, lv_event_cb_t event_cb)
{
    lv_obj_t *bg_title = lv_obj_create(parent);
    lv_obj_remove_style_all(bg_title);
    lv_obj_set_style_bg_opa(bg_title, LV_OPA_COVER, LV_PART_ANY);
	lv_obj_set_scrollbar_mode(bg_title, LV_SCROLLBAR_MODE_OFF);
	lv_obj_remove_flag(bg_title, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_set_size(bg_title, 240, 60);
    lv_obj_set_style_bg_color(bg_title, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_align(bg_title, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_update_layout(bg_title);

    if (event_cb)
    {
        lv_obj_t* back_icons = lv_img_create(bg_title);
        lv_img_set_src(back_icons, &img_guide_back);
        lv_obj_set_pos(back_icons, 20, 8);
        lv_obj_add_event_cb(back_icons, event_cb, LV_EVENT_SHORT_CLICKED, NULL);
        lv_obj_add_flag(back_icons, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
    }

    lv_obj_t *title = lv_label_create(bg_title);
    lv_obj_set_style_text_color(title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_label_set_long_mode(title, LV_LABEL_LONG_DOT);
    lv_obj_set_width(title, 200);
    lv_obj_set_pos(title, 20, 30);
    lv_label_set_text(title, txt);
    lv_obj_add_flag(title, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    return bg_title;
}

