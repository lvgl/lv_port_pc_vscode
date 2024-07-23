/*********************
 *      INCLUDES
 *********************/

#include "guide_recovery.h"
#include "guide_comm.h"


static guide_imgbtn_desc_t guide_imgbtn_num_table[] =
{
    {"1.word", 20, 96},
    {"2.word", 130, 96},
    {"3.word", 20, 126},
    {"4.word", 130, 126},
    {"5.word", 20, 156},
    {"6.word", 130, 156},
    {"7.word", 20, 186},
    {"8.word", 130, 186},
    {"9.word", 20, 216},
    {"10.word", 130, 216},
    {"11.word", 20, 246},
    {"12.word", 130, 246},
};

static guide_recovery_t* p_guide_recovery = NULL;

extern void guide_done_start(void);


static void title_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        guide_recovery_stop();
        guide_ready_check_start();
    }
}

static void guide_recovery_word_btn_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        printf("%s\n", (char *)e->user_data);
    }
}
static void guide_recovery_continue_btn_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        //gui_app_run_subpage("guide", "done", NULL);
        guide_recovery_stop();
        guide_done_start();
    }
}
static void guide_recovery_bg_cont(lv_obj_t* parent)
{
    guide_draw_title(parent, "Recovery Phrase", title_cb);

    lv_obj_t* label_describe = lv_label_create(parent);
    lv_obj_set_style_text_color(label_describe, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_describe, &lv_font_montserrat_12, 0);
    lv_label_set_text(label_describe, "Write down the following 12 Words in orders.");
    lv_label_set_long_mode(label_describe, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label_describe, 200);
    lv_obj_set_pos(label_describe, 20, 60);

    for (uint8_t i = 0; i < sizeof(guide_imgbtn_num_table) / sizeof(guide_imgbtn_desc_t); i++)
    {
        lv_obj_t* img_btn = lv_imagebutton_create(parent);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
        lv_obj_set_width(img_btn, 90);
        lv_obj_set_pos(img_btn, guide_imgbtn_num_table[i].x, guide_imgbtn_num_table[i].y);
        lv_obj_add_event_cb(img_btn, guide_recovery_word_btn_handler, LV_EVENT_SHORT_CLICKED, (void *)guide_imgbtn_num_table[i].str);
        lv_obj_add_flag(img_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t* label = lv_label_create(img_btn);
		lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
		lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
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
    lv_obj_set_style_text_color(label_continue, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_continue, &lv_font_montserrat_12, 0);
    lv_label_set_text(label_continue, "Contiune");
    lv_obj_align(label_continue, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(label_continue, LV_OBJ_FLAG_EVENT_BUBBLE);
}


void guide_recovery_start(void)
{
    p_guide_recovery = (guide_recovery_t*)lv_malloc(sizeof(guide_recovery_t));
    LV_ASSERT(p_guide_recovery);
    lv_memset(p_guide_recovery, 0, sizeof(guide_recovery_t));

    p_guide_recovery->bg_cont = guide_draw_bg_cont();
    guide_recovery_bg_cont(p_guide_recovery->bg_cont);
}

void guide_recovery_stop(void)
{
    lv_obj_del(p_guide_recovery->bg_cont);
    lv_free(p_guide_recovery);
    p_guide_recovery = NULL;
}


