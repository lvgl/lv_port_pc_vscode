/*********************
 *      INCLUDES
 *********************/

#include "view_transaction_confirm.h"
#include "gui_comm.h"

extern void view_transaction_main_start(void);
extern void view_transaction_waiting_start(void);


static view_transaction_confirm_t* p_view_transaction_confirm = NULL;


static void timer_cb(lv_timer_t * tmr)
{
    if(1)
    {
        lv_timer_del(p_view_transaction_confirm->timer);
        view_transaction_confirm_stop();
        view_transaction_waiting_start();
    }
}

static void slider_cb(void)
{
	lv_obj_clear_flag(p_view_transaction_confirm->slider, LV_OBJ_FLAG_CLICKABLE);
	p_view_transaction_confirm->timer = lv_timer_create(timer_cb, 100, NULL);
}

static void reject_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        view_transaction_confirm_stop();
        view_transaction_main_start();
    }
}

static void view_transaction_confirm_bg_cont(lv_obj_t* parent)
{
    lv_obj_t *title = lv_label_create(parent);
    lv_obj_set_style_text_color(title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_18, 0);
    lv_label_set_long_mode(title, LV_LABEL_LONG_DOT);
    lv_obj_set_width(title, 200);
    lv_obj_set_pos(title, 20, 30);
    lv_label_set_text(title, "Confirm Transaction");
    
    lv_obj_t* describe_label = lv_label_create(parent);
    lv_obj_set_style_text_color(describe_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(describe_label, &lv_font_montserrat_12, 0);
    lv_label_set_long_mode(describe_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(describe_label, 200);
    lv_obj_set_pos(describe_label, 20, 60);
    lv_label_set_text(describe_label, "Do you want to sign this () transaction");
    lv_obj_add_flag(describe_label, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

	p_view_transaction_confirm->slider = gui_comm_draw_slider(parent, 232, slider_cb);
	
    lv_obj_t* img_btn_reject = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_reject, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
    lv_imagebutton_set_src(img_btn_reject, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
    lv_obj_set_width(img_btn_reject, 200);
    lv_obj_set_pos(img_btn_reject, 20, 278);
    lv_obj_add_event_cb(img_btn_reject, reject_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_reject, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_reject = lv_label_create(img_btn_reject);
    lv_obj_set_style_text_color(label_reject, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_reject, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_reject, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_reject, LV_LABEL_LONG_DOT);
    lv_obj_set_width(label_reject, 200);
    lv_label_set_text(label_reject, "reject");
    lv_obj_center(label_reject);
    lv_obj_add_flag(label_reject, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

}


void view_transaction_confirm_start(void)
{
    p_view_transaction_confirm = (view_transaction_confirm_t*)lv_malloc(sizeof(view_transaction_confirm_t));
    LV_ASSERT(p_view_transaction_confirm);
    lv_memset(p_view_transaction_confirm, 0, sizeof(view_transaction_confirm_t));

    p_view_transaction_confirm->bg_cont = gui_comm_draw_bg();
    view_transaction_confirm_bg_cont(p_view_transaction_confirm->bg_cont);
}

 void view_transaction_confirm_stop(void)
{
    lv_obj_del(p_view_transaction_confirm->bg_cont);
    lv_free(p_view_transaction_confirm);
    p_view_transaction_confirm = NULL;
}
