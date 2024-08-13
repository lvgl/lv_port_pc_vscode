/*********************
 *      INCLUDES
 *********************/

#include "settings_erase.h"
#include "gui_comm.h"

extern void settings_complete_start(void);
extern void settings_security_start(void);

static settings_erase_t* p_settings_erase = NULL;

static void timer_cb(lv_timer_t * tmr)
{
    if(1)
    {
        lv_timer_del(p_settings_erase->timer);
    	printf("timer_cb\n");
        settings_erase_stop();
        settings_complete_start();
    }
}

static void slider_cb(void)
{
	lv_obj_clear_flag(p_settings_erase->slider, LV_OBJ_FLAG_CLICKABLE);
	p_settings_erase->timer = lv_timer_create(timer_cb, 100, NULL);
}

static void img_check1_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
    	p_settings_erase->check1 = !p_settings_erase->check1;
    	if(p_settings_erase->check1)
			lv_img_set_src(e->original_target, &img_chosen);
		else
			lv_img_set_src(e->original_target, &img_unchosen);
    }
}

static void img_check2_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
    	p_settings_erase->check2 = !p_settings_erase->check2;
    	if(p_settings_erase->check2)
			lv_img_set_src(e->original_target, &img_chosen);
		else
			lv_img_set_src(e->original_target, &img_unchosen);
    }
}

static void cancle_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        settings_erase_stop();
        settings_security_start();
    }
}

static void settings_erase_bg_cont(lv_obj_t* parent)
{
    gui_comm_draw_title(parent, "Erase This Device", NULL);

    lv_obj_t* describe_label = lv_label_create(parent);
    lv_obj_set_style_text_color(describe_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(describe_label, &lv_font_montserrat_12, 0);
    lv_label_set_long_mode(describe_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(describe_label, 200);
    lv_obj_set_pos(describe_label, 20, 60);
    lv_label_set_text(describe_label, "This will erase all data on internal storage and Secure Element (SE). Beforehand, you need to know:");
    lv_obj_add_flag(describe_label, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t *bg_obj_check1 = lv_obj_create(parent);
    lv_obj_remove_style_all(bg_obj_check1);
    lv_obj_set_style_bg_color(bg_obj_check1, lv_color_hex(0x222222), LV_PART_MAIN);
    lv_obj_set_style_border_color(bg_obj_check1, lv_color_hex(0x6C6CF4), LV_PART_MAIN);
    lv_obj_set_style_border_width(bg_obj_check1, 1, LV_PART_MAIN);
    lv_obj_set_style_radius(bg_obj_check1, 6, LV_PART_MAIN);
    lv_obj_set_size(bg_obj_check1, 200, 50);
    lv_obj_align_to(bg_obj_check1, parent, LV_ALIGN_TOP_MID, 0, 113);

    lv_obj_t* img_check1 = lv_img_create(bg_obj_check1);
	lv_img_set_src(img_check1, &img_unchosen);
	lv_obj_align_to(img_check1, bg_obj_check1, LV_ALIGN_LEFT_MID, 9, 0);
    lv_obj_add_event_cb(img_check1, img_check1_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_check1, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_check1= lv_label_create(bg_obj_check1);
    lv_obj_set_style_text_color(label_check1, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_check1, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_check1, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_check1, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label_check1, 160);
    lv_label_set_text(label_check1, "After reset, the recovery phrase on this device will be permanently deleted.");
	lv_obj_align_to(label_check1, bg_obj_check1, LV_ALIGN_LEFT_MID, 30, 0);
    lv_obj_add_flag(label_check1, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t *bg_obj_check2 = lv_obj_create(parent);
    lv_obj_remove_style_all(bg_obj_check2);
    lv_obj_set_style_bg_color(bg_obj_check2, lv_color_hex(0x222222), LV_PART_MAIN);
    lv_obj_set_style_border_color(bg_obj_check2, lv_color_hex(0x6C6CF4), LV_PART_MAIN);
    lv_obj_set_style_border_width(bg_obj_check2, 1, LV_PART_MAIN);
    lv_obj_set_style_radius(bg_obj_check2, 6, LV_PART_MAIN);
    lv_obj_set_size(bg_obj_check2, 200, 50);
    lv_obj_align_to(bg_obj_check2, parent, LV_ALIGN_TOP_MID, 0, 171);

    lv_obj_t* img_check2 = lv_img_create(bg_obj_check2);
	lv_img_set_src(img_check2, &img_unchosen);
	lv_obj_align_to(img_check2, bg_obj_check2, LV_ALIGN_LEFT_MID, 9, 0);
    lv_obj_add_event_cb(img_check2, img_check2_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_check2, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_check2= lv_label_create(bg_obj_check2);
    lv_obj_set_style_text_color(label_check2, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_check2, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_check2, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_check2, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label_check2, 160);
    lv_label_set_text(label_check2, "You can still restore your funds from recovery phrase backup.");
	lv_obj_align_to(label_check2, bg_obj_check2, LV_ALIGN_LEFT_MID, 30, 0);
    lv_obj_add_flag(label_check2, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

	p_settings_erase->slider = gui_comm_draw_slider(parent, 232, slider_cb);
	
    lv_obj_t* img_btn_cancle = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_cancle, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
    lv_imagebutton_set_src(img_btn_cancle, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
    lv_obj_set_width(img_btn_cancle, 200);
    lv_obj_set_pos(img_btn_cancle, 20, 278);
    lv_obj_add_event_cb(img_btn_cancle, cancle_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_cancle, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_cancle = lv_label_create(img_btn_cancle);
    lv_obj_set_style_text_color(label_cancle, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_cancle, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_cancle, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_cancle, LV_LABEL_LONG_DOT);
    lv_obj_set_width(label_cancle, 200);
    lv_label_set_text(label_cancle, "Cancle");
    lv_obj_center(label_cancle);
    lv_obj_add_flag(label_cancle, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
}

void settings_erase_start(void)
{
    p_settings_erase = (settings_erase_t*)lv_malloc(sizeof(settings_erase_t));
    LV_ASSERT(p_settings_erase);
    lv_memset(p_settings_erase, 0, sizeof(settings_erase_t));

    p_settings_erase->bg_cont = gui_comm_draw_bg();
    settings_erase_bg_cont(p_settings_erase->bg_cont);
}

 void settings_erase_stop(void)
{
    lv_obj_del(p_settings_erase->bg_cont);
    lv_free(p_settings_erase);
    p_settings_erase = NULL;
}
