/*********************
 *      INCLUDES
 *********************/

#include "guide_guide.h"
#include "guide_comm.h"


static guide_guide_t* p_guide_guide = NULL;

static guide_imgbtn_desc_t guide_imgbtn_num_table[] =
{
    {"LIKKIM App Tutorial", 20, 66},
    {"What is Recovery Phrase", 20, 96},
};

static void title_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        guide_guide_stop();
        guide_menu_start();
    }
}

static void guide_guide_word_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        printf("setting guide:%s\n", (char*)e->user_data);
    }
}
static void guide_guide_bg_cont(lv_obj_t* parent)
{
    guide_draw_title(parent, "User Guide", title_cb);

    for (uint8_t i = 0; i < sizeof(guide_imgbtn_num_table) / sizeof(guide_imgbtn_desc_t); i++)
    {
        lv_obj_t* img_btn = lv_imagebutton_create(parent);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
        lv_obj_set_width(img_btn, 200);
        lv_obj_set_pos(img_btn, guide_imgbtn_num_table[i].x, guide_imgbtn_num_table[i].y);
        lv_obj_add_event_cb(img_btn, guide_guide_word_handler, LV_EVENT_SHORT_CLICKED, (void *)guide_imgbtn_num_table[i].str);
        lv_obj_add_flag(img_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t* label = lv_label_create(img_btn);
		lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
		lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
        lv_label_set_text(label, guide_imgbtn_num_table[i].str);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    }
}


void guide_guide_start(void)
{
    p_guide_guide = (guide_guide_t*)lv_malloc(sizeof(guide_guide_t));
    LV_ASSERT(p_guide_guide);
    lv_memset(p_guide_guide, 0, sizeof(guide_guide_t));

    p_guide_guide->bg_cont = guide_draw_bg_cont();
    guide_guide_bg_cont(p_guide_guide->bg_cont);
}

void guide_guide_stop(void)
{
    lv_obj_del(p_guide_guide->bg_cont);
    lv_free(p_guide_guide);
    p_guide_guide = NULL;
}


