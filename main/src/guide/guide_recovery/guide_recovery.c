/*********************
 *      INCLUDES
 *********************/

#include "guide_recovery.h"
#include "gui_comm.h"

extern void guide_main_start(void);

static guide_recovery_t* p_guide_recovery = NULL;

static void title_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        guide_recovery_stop();
        guide_main_start();
    }
}

static void guide_recovery_bg_cont(lv_obj_t* parent)
{
    lv_obj_t* back_icons = lv_img_create(parent);
    lv_img_set_src(back_icons, &img_guide_back);
    lv_obj_set_pos(back_icons, 20, 8);
    lv_obj_add_event_cb(back_icons, title_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(back_icons, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t *title = lv_label_create(parent);
    lv_obj_set_style_text_color(title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_12, 0);
    lv_label_set_long_mode(title, LV_LABEL_LONG_DOT);
    lv_obj_set_width(title, 200);
    lv_obj_set_pos(title, 20, 40);
    lv_label_set_text(title, "What is Recovery Phrase");
    lv_obj_add_flag(title, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t *bg_obj = lv_obj_create(parent);
	lv_obj_set_scroll_dir(bg_obj, LV_DIR_VER);
	lv_obj_set_style_bg_color(bg_obj, lv_color_hex(0x888888), LV_PART_MAIN);
    lv_obj_set_size(bg_obj, 200, 238);
    lv_obj_set_pos(bg_obj, 20, 66);
	
    lv_obj_t *label_describe = lv_label_create(bg_obj);
    lv_obj_set_style_text_color(label_describe, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_describe, &lv_font_montserrat_12, 0);
    lv_label_set_long_mode(label_describe, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label_describe, 160);
    lv_label_set_text(label_describe, "Welcome to LIKKIM, your secure cold wallet solution. Welcome to LIKKIM, your secure cold wallet solution.Welcome to LIKKIM, your secure cold wallet solution.Welcome to LIKKIM, your secure cold wallet solution.Welcome to LIKKIM, your secure cold wallet solution.Welcome to LIKKIM, your secure cold wallet solution.");
    lv_obj_add_flag(label_describe, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_center(label_describe);
}


void guide_recovery_start(void)
{
    p_guide_recovery = (guide_recovery_t*)lv_malloc(sizeof(guide_recovery_t));
    LV_ASSERT(p_guide_recovery);
    lv_memset(p_guide_recovery, 0, sizeof(guide_recovery_t));

    p_guide_recovery->bg_cont = gui_comm_draw_bg();
    guide_recovery_bg_cont(p_guide_recovery->bg_cont);
}

void guide_recovery_stop(void)
{
    lv_obj_del(p_guide_recovery->bg_cont);
    lv_free(p_guide_recovery);
    p_guide_recovery = NULL;
}


