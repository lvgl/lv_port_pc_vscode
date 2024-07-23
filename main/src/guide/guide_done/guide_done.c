/*********************
 *      INCLUDES
 *********************/

#include "guide_done.h"
#include "guide_comm.h"


static guide_done_t* p_guide_done = NULL;


static void guide_done_back_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        //gui_app_goback();
        guide_done_stop();
        guide_recovery_start();
    }
}
static void guide_done_continue_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        //gui_app_run_subpage("guide", "verification", NULL);
        guide_done_stop();
        guide_verification_start();
    }
}
static void guide_done_bg_cont(lv_obj_t* parent)
{
    guide_draw_title(parent, "Almost Done!", NULL);

    lv_obj_t* label_describe = lv_label_create(parent);
    lv_obj_set_style_text_color(label_describe, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_describe, &lv_font_montserrat_12, 0);
    lv_label_set_text(label_describe, "Next, Following the guide and check words one by one, based on your recovery phrase backup.");
    lv_label_set_long_mode(label_describe, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label_describe, 200);
    lv_obj_set_pos(label_describe, 20, 60);

    lv_obj_t* img_btn_back = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_back, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
    lv_imagebutton_set_src(img_btn_back, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
    lv_obj_set_width(img_btn_back, 96);
    lv_obj_set_pos(img_btn_back, 20, 278);
    lv_obj_add_event_cb(img_btn_back, guide_done_back_handler, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_back, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_back = lv_label_create(img_btn_back);
    lv_obj_set_style_text_color(label_back, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_back, &lv_font_montserrat_12, 0);
    lv_label_set_text(label_back, "back");
    lv_obj_align(label_back, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(label_back, LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t* img_btn_continue = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_continue, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_imagebutton_set_src(img_btn_continue, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_obj_set_width(img_btn_continue, 96);
    lv_obj_set_pos(img_btn_continue, 124, 278);
    lv_obj_add_event_cb(img_btn_continue, guide_done_continue_handler, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_continue, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_continue = lv_label_create(img_btn_continue);
    lv_obj_set_style_text_color(label_continue, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_continue, &lv_font_montserrat_12, 0);
    lv_label_set_text(label_continue, "Contiune");
    lv_obj_align(label_continue, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(label_continue, LV_OBJ_FLAG_EVENT_BUBBLE);
}


void guide_done_start(void)
{
    p_guide_done = (guide_done_t*)lv_malloc(sizeof(guide_done_t));
    LV_ASSERT(p_guide_done);
    lv_memset(p_guide_done, 0, sizeof(guide_done_t));

    p_guide_done->bg_cont = guide_draw_bg_cont();
    guide_done_bg_cont(p_guide_done->bg_cont);
}

void guide_done_stop(void)
{
    lv_obj_del(p_guide_done->bg_cont);
    lv_free(p_guide_done);
    p_guide_done = NULL;
}

