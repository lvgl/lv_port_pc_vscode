/*********************
 *      INCLUDES
 *********************/

#include "guide_general.h"
#include "guide_comm.h"


static guide_general_t* p_guide_general = NULL;

static guide_imgbtn_desc_t guide_imgbtn_num_table[] =
{
    {"Auto-Lock", 20, 66},
    {"Language", 20, 96},
    {"Brighness", 20, 126},
    {"Lock Screen", 20, 156},
};

static void title_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        guide_general_stop();
        guide_menu_start();
    }
}

static void guide_general_word_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        printf("setting general:%s\n", (char*)e->user_data);
    }
}

static void guide_general_bg_cont(lv_obj_t* parent)
{
    guide_draw_title(parent, "General", title_cb);

    for (uint8_t i = 0; i < sizeof(guide_imgbtn_num_table) / sizeof(guide_imgbtn_desc_t); i++)
    {
        lv_obj_t* img_btn = lv_imagebutton_create(parent);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
        lv_obj_set_width(img_btn, 200);
        lv_obj_set_pos(img_btn, guide_imgbtn_num_table[i].x, guide_imgbtn_num_table[i].y);
        lv_obj_add_event_cb(img_btn, guide_general_word_handler, LV_EVENT_SHORT_CLICKED, (void *)guide_imgbtn_num_table[i].str);
        lv_obj_add_flag(img_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t* label = lv_label_create(img_btn);
		lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
		lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
        lv_label_set_text(label, guide_imgbtn_num_table[i].str);
        lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, 2);

        if (0 == i)
        {
            lv_obj_t* labe_detail = lv_label_create(img_btn);
			lv_obj_set_style_text_color(labe_detail, lv_color_hex(0xffffff), 0);
			lv_obj_set_style_text_font(labe_detail, &lv_font_montserrat_12, 0);
            lv_label_set_text(labe_detail, "10 minutes");
            lv_obj_align(labe_detail, LV_ALIGN_RIGHT_MID, -17, 2);
        }
        else if (1 == i)
        {
            lv_obj_t* labe_detail = lv_label_create(img_btn);
			lv_obj_set_style_text_color(labe_detail, lv_color_hex(0xffffff), 0);
			lv_obj_set_style_text_font(labe_detail, &lv_font_montserrat_12, 0);
            lv_label_set_text(labe_detail, "English");
            lv_obj_align(labe_detail, LV_ALIGN_RIGHT_MID, -17, 2);
        }
        else if (2 == i)
        {
            lv_obj_t* labe_detail = lv_label_create(img_btn);
			lv_obj_set_style_text_color(labe_detail, lv_color_hex(0xffffff), 0);
			lv_obj_set_style_text_font(labe_detail, &lv_font_montserrat_12, 0);
            lv_label_set_text(labe_detail, "50%");
            lv_obj_align(labe_detail, LV_ALIGN_RIGHT_MID, -17, 2);
        }
        else
            lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    }
}


void guide_general_start(void)
{
    p_guide_general = (guide_general_t*)lv_malloc(sizeof(guide_general_t));
    LV_ASSERT(p_guide_general);
    lv_memset(p_guide_general, 0, sizeof(guide_general_t));

    p_guide_general->bg_cont = guide_draw_bg_cont();
    guide_general_bg_cont(p_guide_general->bg_cont);
}

void guide_general_stop(void)
{
    lv_obj_del(p_guide_general->bg_cont);
    lv_free(p_guide_general);
    p_guide_general = NULL;
}


