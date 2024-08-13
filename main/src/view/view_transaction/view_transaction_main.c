/*********************
 *      INCLUDES
 *********************/

#include "view_transaction_main.h"
#include "gui_comm.h"

extern void settings_security_start(void);
extern void view_transaction_confirm_start(void);


static view_transaction_main_t* p_view_transaction_main = NULL;

static void continue_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        view_transaction_main_stop();
		view_transaction_confirm_start();
    }
}

static void reject_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        view_transaction_main_stop();
        settings_security_start();
    }
}

static void view_transaction_main_bg_cont(lv_obj_t* parent)
{
    lv_obj_t* title = lv_label_create(parent);
    lv_obj_set_style_text_color(title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_label_set_long_mode(title, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(title, 200);
    lv_obj_set_pos(title, 20, 30);
    lv_label_set_text(title, "View Transaction");
/*滑动区域*/

/*第一个滑动框*/
	lv_obj_t *bg_obj = lv_obj_create(parent);
	lv_obj_remove_style_all(bg_obj);
	lv_obj_set_scrollbar_mode(bg_obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_bg_color(bg_obj, lv_color_hex(0x000000), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(bg_obj, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_size(bg_obj, 240, 170);
	lv_obj_set_pos(bg_obj, 0, 66);

	lv_obj_t *bg_obj_directions = lv_obj_create(bg_obj);
	lv_obj_remove_style_all(bg_obj_directions);
	lv_obj_set_scrollbar_mode(bg_obj_directions, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_bg_color(bg_obj_directions, lv_color_hex(0x888888), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(bg_obj_directions, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_style_radius(bg_obj_directions, 6, LV_PART_MAIN);
	lv_obj_set_pos(bg_obj_directions, 20, 0);

	lv_obj_t *obj_line1 = lv_obj_create(bg_obj_directions);
	lv_obj_set_style_bg_color(obj_line1, lv_color_hex(0x475358), LV_PART_MAIN);
	lv_obj_set_size(obj_line1, 160, 1);
	lv_obj_set_pos(obj_line1, 20, 28);

	lv_obj_t* label_directions = lv_label_create(bg_obj_directions);
	lv_obj_set_style_text_color(label_directions, lv_color_hex(0xffffff), 0);
	lv_obj_set_style_text_font(label_directions, &lv_font_montserrat_12, 0);
	lv_label_set_long_mode(label_directions, LV_LABEL_LONG_WRAP);
	lv_obj_set_width(label_directions, 168);
	lv_obj_set_pos(label_directions, 20, 5);
	lv_label_set_text(label_directions, "Directions");
	lv_obj_update_layout(label_directions);

	lv_obj_t* label_fee = lv_label_create(bg_obj_directions);
	lv_obj_set_style_text_color(label_fee, lv_color_hex(0xffffff), 0);
	lv_obj_set_style_text_font(label_fee, &lv_font_montserrat_12, 0);
	lv_label_set_long_mode(label_fee, LV_LABEL_LONG_WRAP);
	lv_obj_set_width(label_fee, 168);
	lv_obj_set_pos(label_fee, 20, 34);
	lv_label_set_text(label_fee, "Fee Payer");
	lv_obj_update_layout(label_fee);

	lv_obj_t* label_fee_content = lv_label_create(bg_obj_directions);
	lv_obj_set_style_text_color(label_fee_content, lv_color_hex(0xffffff), 0);
	lv_obj_set_style_text_font(label_fee_content, &lv_font_montserrat_12, 0);
	lv_label_set_long_mode(label_fee_content, LV_LABEL_LONG_WRAP);
	lv_obj_set_width(label_fee_content, 168);
	lv_obj_set_pos(label_fee_content, 20, 52);
	lv_label_set_text(label_fee_content, "label_fee_contentlabel_fee_contentlabel_fee_contentlabel_fee_content");
	lv_obj_update_layout(label_fee_content);
	lv_obj_set_size(bg_obj_directions, 200, lv_obj_get_height(label_fee_content) + 52 + 5);

/*第二个滑动框*/
	lv_obj_t *bg_obj_more = lv_obj_create(bg_obj);
	lv_obj_remove_style_all(bg_obj_more);
	lv_obj_set_scrollbar_mode(bg_obj_more, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_bg_color(bg_obj_more, lv_color_hex(0x888888), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(bg_obj_more, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_style_radius(bg_obj_more, 6, LV_PART_MAIN);

	lv_obj_t *obj_line2 = lv_obj_create(bg_obj_more);
	lv_obj_set_style_bg_color(obj_line2, lv_color_hex(0x475358), LV_PART_MAIN);
	lv_obj_set_size(obj_line2, 160, 1);
	lv_obj_set_pos(obj_line2, 20, 28);

	lv_obj_t* label_more = lv_label_create(bg_obj_more);
	lv_obj_set_style_text_color(label_more, lv_color_hex(0xffffff), 0);
	lv_obj_set_style_text_font(label_more, &lv_font_montserrat_12, 0);
	lv_label_set_long_mode(label_more, LV_LABEL_LONG_WRAP);
	lv_obj_set_width(label_more, 168);
	lv_obj_set_pos(label_more, 20, 5);
	lv_label_set_text(label_more, "More");
	lv_obj_update_layout(label_more);

	lv_obj_t* label_format = lv_label_create(bg_obj_more);
	lv_obj_set_style_text_color(label_format, lv_color_hex(0xffffff), 0);
	lv_obj_set_style_text_font(label_format, &lv_font_montserrat_12, 0);
	lv_label_set_long_mode(label_format, LV_LABEL_LONG_WRAP);
	lv_obj_set_width(label_format, 168);
	lv_obj_set_pos(label_format, 20, 34);
	lv_label_set_text(label_format, "Format");
	lv_obj_update_layout(label_format);

	lv_obj_t* label_format_content = lv_label_create(bg_obj_more);
	lv_obj_set_style_text_color(label_format_content, lv_color_hex(0xffffff), 0);
	lv_obj_set_style_text_font(label_format_content, &lv_font_montserrat_12, 0);
	lv_label_set_long_mode(label_format_content, LV_LABEL_LONG_WRAP);
	lv_obj_set_width(label_format_content, 168);
	lv_obj_set_pos(label_format_content, 20, 52);
	lv_label_set_text(label_format_content, "label_format_contentlabel_format_contentlabel_format_contentlabel_format_content");
	lv_obj_update_layout(label_format_content);

	lv_obj_t* label_hash = lv_label_create(bg_obj_more);
	lv_obj_set_style_text_color(label_hash, lv_color_hex(0xffffff), 0);
	lv_obj_set_style_text_font(label_hash, &lv_font_montserrat_12, 0);
	lv_label_set_long_mode(label_hash, LV_LABEL_LONG_WRAP);
	lv_obj_set_width(label_hash, 168);
	lv_obj_align_to(label_hash, label_format_content, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
	lv_label_set_text(label_hash, "Format");
	lv_obj_update_layout(label_hash);

	lv_obj_t* label_hash_content = lv_label_create(bg_obj_more);
	lv_obj_set_style_text_color(label_hash_content, lv_color_hex(0xffffff), 0);
	lv_obj_set_style_text_font(label_hash_content, &lv_font_montserrat_12, 0);
	lv_label_set_long_mode(label_hash_content, LV_LABEL_LONG_WRAP);
	lv_obj_set_width(label_hash_content, 168);
	lv_obj_align_to(label_hash_content, label_hash, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
	lv_label_set_text(label_hash_content, "label_format_contentlabel_format_contentlabel_format_contentlabel_format_content");
	lv_obj_update_layout(label_hash_content);

	lv_obj_set_size(bg_obj_more, 200, label_hash_content->coords.y2 + 5);
	lv_obj_align_to(bg_obj_more, bg_obj_directions, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);



/*底部按键*/
    lv_obj_t* img_btn_continue = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_continue, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_imagebutton_set_src(img_btn_continue, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_obj_set_width(img_btn_continue, 200);
    lv_obj_set_pos(img_btn_continue, 20, 246);
    lv_obj_add_event_cb(img_btn_continue, continue_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_continue, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_continue = lv_label_create(img_btn_continue);
    lv_obj_set_style_text_color(label_continue, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_continue, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_continue, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_continue, LV_LABEL_LONG_DOT);
    lv_obj_set_width(label_continue, 200);
    lv_label_set_text(label_continue, "continue");
    lv_obj_center(label_continue);
    lv_obj_add_flag(label_continue, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

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


void view_transaction_main_start(void)
{
    p_view_transaction_main = (view_transaction_main_t*)lv_malloc(sizeof(view_transaction_main_t));
    LV_ASSERT(p_view_transaction_main);
    lv_memset(p_view_transaction_main, 0, sizeof(view_transaction_main_t));

    p_view_transaction_main->bg_cont = gui_comm_draw_bg();
    view_transaction_main_bg_cont(p_view_transaction_main->bg_cont);
}

 void view_transaction_main_stop(void)
{
    lv_obj_del(p_view_transaction_main->bg_cont);
    lv_free(p_view_transaction_main);
    p_view_transaction_main = NULL;
}
