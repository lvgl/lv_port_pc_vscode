/*********************
 *      INCLUDES
 *********************/

#include "startup_ready_check.h"
#include "gui_comm.h"
#include "gui_data_comm.h"


extern void startup_quick_start_start(void);
extern void startup_recovery_start(void);


static startup_ready_check_t* p_startup_ready_check = NULL;


static void title_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        startup_ready_check_stop();
        startup_quick_start_start();
    }
}

static void startup_btn_event_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);
    if (LV_EVENT_SHORT_CLICKED == event)
    {
		int ret = atoi(e->user_data);

		printf("%s, ret : %d\n", (char *)e->user_data, ret);
		gui_data_set_word_num((uint8_t)ret);
		
        startup_ready_check_stop();
        startup_recovery_start();
    }
}

static void startup_ready_check_bg_cont(lv_obj_t* parent)
{
    gui_comm_draw_title(parent, "Ready to Check", title_cb);

    lv_obj_t* describe_label = lv_label_create(parent);
    lv_obj_set_style_text_color(describe_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(describe_label, &lv_font_montserrat_12, 0);
    lv_label_set_long_mode(describe_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(describe_label, 200);
    lv_obj_set_pos(describe_label, 20, 60);
    lv_label_set_text(describe_label, "Select the number of words.");
    lv_obj_add_flag(describe_label, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t* img_btn_words12 = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_words12, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
    lv_imagebutton_set_src(img_btn_words12, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
    lv_obj_set_width(img_btn_words12, 200);
    lv_obj_set_pos(img_btn_words12, 20, 81);
    lv_obj_add_event_cb(img_btn_words12, startup_btn_event_handler, LV_EVENT_SHORT_CLICKED, "12");
    lv_obj_add_flag(img_btn_words12, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_words12 = lv_label_create(img_btn_words12);
    lv_obj_set_style_text_color(label_words12, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_words12, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_words12, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_words12, LV_LABEL_LONG_DOT);
    lv_obj_set_width(label_words12, 200);
    lv_label_set_text(label_words12, "12 Words");
    lv_obj_center(label_words12);
    lv_obj_add_flag(label_words12, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t* img_btn_words18 = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_words18, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
    lv_imagebutton_set_src(img_btn_words18, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
    lv_obj_set_width(img_btn_words18, 200);
    lv_obj_set_pos(img_btn_words18, 20, 111);
    lv_obj_add_event_cb(img_btn_words18, startup_btn_event_handler, LV_EVENT_SHORT_CLICKED, "18");
    lv_obj_add_flag(img_btn_words18, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_words18 = lv_label_create(img_btn_words18);
    lv_obj_set_style_text_color(label_words18, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_words18, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_words18, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_words18, LV_LABEL_LONG_DOT);
    lv_obj_set_width(label_words18, 200);
    lv_label_set_text(label_words18, "18 Words");
    lv_obj_center(label_words18);
    lv_obj_add_flag(label_words18, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t* img_btn_words24 = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_words24, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
    lv_imagebutton_set_src(img_btn_words24, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
    lv_obj_set_width(img_btn_words24, 200);
    lv_obj_set_pos(img_btn_words24, 20, 141);
    lv_obj_add_event_cb(img_btn_words24, startup_btn_event_handler, LV_EVENT_SHORT_CLICKED, "24");
    lv_obj_add_flag(img_btn_words24, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_words24 = lv_label_create(img_btn_words24);
    lv_obj_set_style_text_color(label_words24, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_words24, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_words24, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_words24, LV_LABEL_LONG_DOT);
    lv_obj_set_width(label_words24, 200);
    lv_label_set_text(label_words24, "24 Words");
    lv_obj_center(label_words24);
    lv_obj_add_flag(label_words24, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
}


void startup_ready_check_start(void)
{
    p_startup_ready_check = (startup_ready_check_t*)lv_malloc(sizeof(startup_ready_check_t));
    LV_ASSERT(p_startup_ready_check);
    lv_memset(p_startup_ready_check, 0, sizeof(startup_ready_check_t));

    p_startup_ready_check->bg_cont = gui_comm_draw_bg();
    startup_ready_check_bg_cont(p_startup_ready_check->bg_cont);
}

void startup_ready_check_stop(void)
{
    lv_obj_del(p_startup_ready_check->bg_cont);
    lv_free(p_startup_ready_check);
    p_startup_ready_check = NULL;
}


