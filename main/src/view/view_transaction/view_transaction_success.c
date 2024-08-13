/*********************
 *      INCLUDES
 *********************/

#include "view_transaction_success.h"
#include "gui_comm.h"

extern void settings_security_start(void);



static view_transaction_success_t* p_view_transaction_success = NULL;


static void done_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        view_transaction_success_stop();
        settings_security_start();
    }
}

static void view_transaction_success_bg_cont(lv_obj_t* parent)
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
    lv_label_set_text(title_label, "Transaction Successful");
    
    lv_obj_t* img_btn_Done = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_Done, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_10e807_14x26, &img_mid_released_10e807_4x26, &img_right_released_10e807_14x26);
    lv_imagebutton_set_src(img_btn_Done, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_61fc5a_14x26, &img_mid_pressed_61fc5a_4x26, &img_right_pressed_61fc5a_14x26);
    lv_obj_set_width(img_btn_Done, 200);
    lv_obj_set_pos(img_btn_Done, 20, 278);
    lv_obj_add_event_cb(img_btn_Done, done_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_Done, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_Done = lv_label_create(img_btn_Done);
    lv_obj_set_style_text_color(label_Done, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_Done, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_Done, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_Done, LV_LABEL_LONG_DOT);
    lv_obj_set_width(label_Done, 200);
    lv_label_set_text(label_Done, "done");
    lv_obj_center(label_Done);
    lv_obj_add_flag(label_Done, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
}


void view_transaction_success_start(void)
{
    p_view_transaction_success = (view_transaction_success_t*)lv_malloc(sizeof(view_transaction_success_t));
    LV_ASSERT(p_view_transaction_success);
    lv_memset(p_view_transaction_success, 0, sizeof(view_transaction_success_t));

    p_view_transaction_success->bg_cont = gui_comm_draw_bg();
    view_transaction_success_bg_cont(p_view_transaction_success->bg_cont);
}

 void view_transaction_success_stop(void)
{
    lv_obj_del(p_view_transaction_success->bg_cont);
    lv_free(p_view_transaction_success);
    p_view_transaction_success = NULL;
}
