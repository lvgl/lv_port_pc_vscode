﻿/*********************
 *      INCLUDES
 *********************/

#include "guide_verification.h"
#include "guide_comm.h"


static guide_verification_t* p_guide_verification = NULL;

static guide_imgbtn_desc_t guide_imgbtn_num_table[] =
{
    {"word", 20, 81},
    {"word", 130, 81},
    {"word", 20, 111},
    {"word", 130, 111},
    {"word", 20, 164},
    {"word", 130, 164},
    {"word", 20, 194},
    {"word", 130, 194},
};

static void guide_verification_word_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        printf("setting verification:%s\n", (char*)e->user_data);
    }
}
static void guide_recovery_continue_btn_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        /*这里可能要根据设置的word数量动态调整，先写死12个word*/
        if (12 == p_guide_verification->page_id)
        {
            //gui_app_run_subpage("guide", "menu", NULL);
			guide_menu_start();
			guide_verification_stop();
            return;
        }
        lv_label_set_text_fmt(p_guide_verification->label_describe1, "Verification Word#%d", ++p_guide_verification->page_id);
        lv_label_set_text_fmt(p_guide_verification->label_describe2, "Verification Word#%d", ++p_guide_verification->page_id);

    }
}
static void guide_verification_bg_cont(lv_obj_t* parent)
{
    guide_draw_title(parent, "Verification Phrase", NULL);

    lv_obj_t* label_describe1 = lv_label_create(parent);
	lv_obj_set_style_text_font(label_describe1, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(label_describe1, lv_color_hex(0xffffff), 0);
    lv_label_set_text_fmt(label_describe1, "Verification Word#%d", ++p_guide_verification->page_id);
    lv_label_set_long_mode(label_describe1, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label_describe1, 200);
    lv_obj_set_pos(label_describe1, 20, 60);
    p_guide_verification->label_describe1 = label_describe1;

    lv_obj_t* label_describe2 = lv_label_create(parent);
	lv_obj_set_style_text_font(label_describe2, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(label_describe2, lv_color_hex(0xffffff), 0);
    lv_label_set_text_fmt(label_describe2, "Verification Word#%d", ++p_guide_verification->page_id);
    lv_label_set_long_mode(label_describe2, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label_describe2, 200);
    lv_obj_set_pos(label_describe2, 20, 143);
    p_guide_verification->label_describe2 = label_describe2;

    for (uint8_t i = 0; i < sizeof(guide_imgbtn_num_table) / sizeof(guide_imgbtn_desc_t); i++)
    {
        lv_obj_t* img_btn = lv_imagebutton_create(parent);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
        lv_obj_set_width(img_btn, 90);
        lv_obj_set_pos(img_btn, guide_imgbtn_num_table[i].x, guide_imgbtn_num_table[i].y);
        lv_obj_add_event_cb(img_btn, guide_verification_word_handler, LV_EVENT_SHORT_CLICKED, (void *)guide_imgbtn_num_table[i].str);
        lv_obj_add_flag(img_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t* label = lv_label_create(img_btn);
		lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
		lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
        lv_label_set_text(label, guide_imgbtn_num_table[i].str);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    }

    lv_obj_t* img_btn_continue = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_continue, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_imagebutton_set_src(img_btn_continue, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_obj_set_width(img_btn_continue, 200);
    lv_obj_set_pos(img_btn_continue, 20, 278);
    lv_obj_add_event_cb(img_btn_continue, guide_recovery_continue_btn_handler, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_continue, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_continue = lv_label_create(img_btn_continue);
	lv_obj_set_style_text_font(label_continue, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(label_continue, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label_continue, "Contiune");
    lv_obj_align(label_continue, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(label_continue, LV_OBJ_FLAG_EVENT_BUBBLE);
}


void guide_verification_start(void)
{
    p_guide_verification = (guide_verification_t*)lv_malloc(sizeof(guide_verification_t));
    LV_ASSERT(p_guide_verification);
    lv_memset(p_guide_verification, 0, sizeof(guide_verification_t));

    p_guide_verification->bg_cont = guide_draw_bg_cont();
    guide_verification_bg_cont(p_guide_verification->bg_cont);
}

void guide_verification_stop(void)
{
    lv_obj_del(p_guide_verification->bg_cont);
    lv_free(p_guide_verification);
    p_guide_verification = NULL;
}


