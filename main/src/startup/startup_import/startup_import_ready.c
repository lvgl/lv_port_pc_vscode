/*********************
 *      INCLUDES
 *********************/

#include "startup_import_ready.h"
#include "gui_comm.h"

extern void menu_main_start(void);

static startup_import_ready_t* p_startup_import_ready = NULL;

static void continue_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
    	startup_import_ready_stop();
		menu_main_start();
    }
}
static void startup_import_ready_bg_cont(lv_obj_t* parent)
{
    lv_obj_t* img_succ = lv_img_create(parent);
	lv_img_set_src(img_succ, &img_success);
    lv_obj_set_pos(img_succ, 20, 35);

    lv_obj_t* title_label = lv_label_create(parent);
    lv_obj_set_style_text_color(title_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, 0);
    lv_label_set_long_mode(title_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(title_label, 200);
    lv_obj_set_pos(title_label, 45, 30);
    lv_label_set_text(title_label, "Wallet is Ready");
    
    lv_obj_t* describe_label = lv_label_create(parent);
    lv_obj_set_style_text_color(describe_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(describe_label, &lv_font_montserrat_12, 0);
    lv_label_set_long_mode(describe_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(describe_label, 200);
    lv_obj_set_pos(describe_label, 20, 60);
    lv_label_set_text(describe_label, "You have successfully recovered your wallet.");
    
    lv_obj_t* img_btn_continue = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_continue, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_10e807_14x26, &img_mid_released_10e807_4x26, &img_right_released_10e807_14x26);
    lv_imagebutton_set_src(img_btn_continue, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_61fc5a_14x26, &img_mid_pressed_61fc5a_4x26, &img_right_pressed_61fc5a_14x26);
    lv_obj_set_width(img_btn_continue, 200);
    lv_obj_set_pos(img_btn_continue, 20, 278);
    lv_obj_add_event_cb(img_btn_continue, continue_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_continue, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_continue = lv_label_create(img_btn_continue);
    lv_obj_set_style_text_color(label_continue, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_continue, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_continue, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_continue, LV_LABEL_LONG_DOT);
    lv_obj_set_width(label_continue, 200);
    lv_label_set_text(label_continue, "Continue");
    lv_obj_center(label_continue);
    lv_obj_add_flag(label_continue, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
}


void startup_import_ready_start(void)
{
    p_startup_import_ready = (startup_import_ready_t*)lv_malloc(sizeof(startup_import_ready_t));
    LV_ASSERT(p_startup_import_ready);
    lv_memset(p_startup_import_ready, 0, sizeof(startup_import_ready_t));

    p_startup_import_ready->bg_cont = gui_comm_draw_bg();
    startup_import_ready_bg_cont(p_startup_import_ready->bg_cont);
}

void startup_import_ready_stop(void)
{
    lv_obj_del(p_startup_import_ready->bg_cont);
    lv_free(p_startup_import_ready);
    p_startup_import_ready = NULL;
}


