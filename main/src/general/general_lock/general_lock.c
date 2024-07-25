/*********************
 *      INCLUDES
 *********************/

#include "general_lock.h"
#include "gui_comm.h"

extern void general_main_start(void);

static general_lock_t* p_general_lock = NULL;

static gui_comm_imgbtn_desc_t gui_comm_imgbtn_num_table[] =
{
    {"5 minutes", 20, 66},
    {"10 minutes", 20, 96},
    {"15 minutes", 20, 126},
    {"30 minutes", 20, 156},
    {"45 minutes", 20, 186},
};

static void title_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        general_lock_stop();
        general_main_start();
    }
}

static void general_lock_word_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        printf("lock time:%s\n", (char*)e->user_data);
    }
}

static void general_lock_bg_cont(lv_obj_t* parent)
{
    gui_comm_draw_title(parent, "Auto-Lock", title_cb);

    for (uint8_t i = 0; i < sizeof(gui_comm_imgbtn_num_table) / sizeof(gui_comm_imgbtn_desc_t); i++)
    {
        lv_obj_t* img_btn = lv_imagebutton_create(parent);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
        lv_obj_set_width(img_btn, 200);
        lv_obj_set_pos(img_btn, gui_comm_imgbtn_num_table[i].x, gui_comm_imgbtn_num_table[i].y);
        lv_obj_add_event_cb(img_btn, general_lock_word_handler, LV_EVENT_SHORT_CLICKED, (void *)gui_comm_imgbtn_num_table[i].str);
        lv_obj_add_flag(img_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t* label = lv_label_create(img_btn);
		lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
		lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
        lv_label_set_text(label, gui_comm_imgbtn_num_table[i].str);
        lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, 2);
    }
}


void general_lock_start(void)
{
    p_general_lock = (general_lock_t*)lv_malloc(sizeof(general_lock_t));
    LV_ASSERT(p_general_lock);
    lv_memset(p_general_lock, 0, sizeof(general_lock_t));

    p_general_lock->bg_cont = gui_comm_draw_bg();
    general_lock_bg_cont(p_general_lock->bg_cont);
}

void general_lock_stop(void)
{
    lv_obj_del(p_general_lock->bg_cont);
    lv_free(p_general_lock);
    p_general_lock = NULL;
}


