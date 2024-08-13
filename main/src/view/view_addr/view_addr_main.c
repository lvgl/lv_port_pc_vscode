/*********************
 *      INCLUDES
 *********************/

#include "view_addr_main.h"
#include "gui_comm.h"

extern void view_addr_qrcode_start(void);

static void view_text(lv_obj_t* parent);
static void view_qrcode(lv_obj_t* parent);


static view_addr_main_t* p_view_addr_main = NULL;

static void done_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
    	printf("%s\n", __func__);
    	view_addr_main_stop();
    }
}

static void qrcode_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
    	if(p_view_addr_main->page_mode == VIEW_ADDR_MODE_TEXT)
    	{
    		view_qrcode(p_view_addr_main->bg_cont);
			lv_label_set_text(p_view_addr_main->label_qrcode, "Address");
    	}
    	else if(p_view_addr_main->page_mode == VIEW_ADDR_MODE_QRCODE)
    	{
    		view_text(p_view_addr_main->bg_cont);
			lv_label_set_text(p_view_addr_main->label_qrcode, "QR code");
    	}

    	p_view_addr_main->page_mode = !p_view_addr_main->page_mode;
    }
}
static void view_text(lv_obj_t* parent)
{
	if(p_view_addr_main->bg_cont_qrcode)
		lv_obj_add_flag(p_view_addr_main->bg_cont_qrcode, LV_OBJ_FLAG_HIDDEN);
		
	if(p_view_addr_main->bg_cont_text)
		lv_obj_clear_flag(p_view_addr_main->bg_cont_text, LV_OBJ_FLAG_HIDDEN);
	else
	{
	    lv_obj_t *bg_obj = lv_obj_create(parent);
	    lv_obj_remove_style_all(bg_obj);
		lv_obj_set_scrollbar_mode(bg_obj, LV_SCROLLBAR_MODE_OFF);
	    lv_obj_set_style_bg_color(bg_obj, lv_color_hex(0x000000), LV_PART_MAIN);
		lv_obj_set_style_bg_opa(bg_obj, LV_OPA_COVER, LV_PART_MAIN);
	    lv_obj_set_size(bg_obj, 240, 170);
	    lv_obj_set_pos(bg_obj, 0, 66);
		p_view_addr_main->bg_cont_text = bg_obj;
	
	    lv_obj_t *bg_obj_addr = lv_obj_create(bg_obj);
		lv_obj_set_scrollbar_mode(bg_obj_addr, LV_SCROLLBAR_MODE_OFF);
	    lv_obj_set_style_bg_color(bg_obj_addr, lv_color_hex(0x888888), LV_PART_MAIN);
	    lv_obj_set_style_radius(bg_obj_addr, 15, LV_PART_MAIN);
		lv_obj_set_pos(bg_obj_addr, 20, 0);

	    lv_obj_t* label_addr = lv_label_create(bg_obj);
	    lv_obj_set_style_text_color(label_addr, lv_color_hex(0xffffff), 0);
	    lv_obj_set_style_text_font(label_addr, &lv_font_montserrat_20, 0);
	    lv_label_set_long_mode(label_addr, LV_LABEL_LONG_WRAP);
	    lv_obj_set_width(label_addr, 168);
	    lv_obj_set_pos(label_addr, 30, 10);
	    lv_label_set_text(label_addr, "AddressAddressAddressAddressAddressAddressAddressAddressAddressAddressAddressAddressAddress");
		lv_obj_update_layout(label_addr);
		
	    lv_obj_set_size(bg_obj_addr, 200, lv_obj_get_height(label_addr) + 20);

	    lv_obj_t *bg_obj_path = lv_obj_create(bg_obj);
		lv_obj_set_scrollbar_mode(bg_obj_path, LV_SCROLLBAR_MODE_OFF);
	    lv_obj_set_style_bg_color(bg_obj_path, lv_color_hex(0x888888), LV_PART_MAIN);
	    lv_obj_set_style_radius(bg_obj_path, 15, LV_PART_MAIN);
	    
	    lv_obj_t* label_path = lv_label_create(bg_obj);
	    lv_obj_set_style_text_color(label_path, lv_color_hex(0xffffff), 0);
	    lv_obj_set_style_text_font(label_path, &lv_font_montserrat_20, 0);
	    lv_label_set_long_mode(label_path, LV_LABEL_LONG_WRAP);
	    lv_obj_set_width(label_path, 168);
	    lv_obj_align_to(label_path, bg_obj_addr, LV_ALIGN_OUT_BOTTOM_MID, 0, 17);
	    lv_label_set_text(label_path, "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
		lv_obj_update_layout(label_path);

	    lv_obj_set_size(bg_obj_path, 200, lv_obj_get_height(label_path) + 20);
	    lv_obj_align_to(bg_obj_path, bg_obj_addr, LV_ALIGN_OUT_BOTTOM_MID, 0, 7);
    }
}

static void view_qrcode(lv_obj_t* parent)
{	
	if(p_view_addr_main->bg_cont_text)
		lv_obj_add_flag(p_view_addr_main->bg_cont_text, LV_OBJ_FLAG_HIDDEN);
		
	if(p_view_addr_main->bg_cont_qrcode)
		lv_obj_clear_flag(p_view_addr_main->bg_cont_qrcode, LV_OBJ_FLAG_HIDDEN);
	else
	{
		lv_obj_t *bg_obj = lv_obj_create(parent);
		lv_obj_remove_style_all(bg_obj);
		lv_obj_set_scrollbar_mode(bg_obj, LV_SCROLLBAR_MODE_OFF);
		lv_obj_set_style_bg_opa(bg_obj, LV_OPA_COVER, LV_PART_MAIN);
		lv_obj_set_style_bg_color(bg_obj, lv_color_hex(0xffffff), LV_PART_MAIN);
		lv_obj_set_style_radius(bg_obj, 16, LV_PART_MAIN);
		lv_obj_set_size(bg_obj, 177, 177);
		lv_obj_set_pos(bg_obj, 32, 59);
		p_view_addr_main->bg_cont_qrcode = bg_obj;

		lv_obj_t *qrcode = lv_qrcode_create(bg_obj);
		lv_qrcode_set_light_color(qrcode, lv_color_hex(0xffffff));
		lv_qrcode_set_dark_color(qrcode, lv_color_hex(0x000000));
		lv_qrcode_set_size(qrcode, 160);
		lv_qrcode_update(qrcode, "faefawefawef", 12);
		lv_obj_center(qrcode);
	}
}

static void view_addr_main_bg_cont(lv_obj_t* parent)
{
    lv_obj_t* title = lv_label_create(parent);
    lv_obj_set_style_text_color(title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_label_set_long_mode(title, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(title, 200);
    lv_obj_set_pos(title, 20, 30);
    lv_label_set_text(title, "Address");

	view_qrcode(parent);
    view_text(parent);

    lv_obj_t* img_btn_confirm = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_confirm, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_imagebutton_set_src(img_btn_confirm, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_obj_set_width(img_btn_confirm, 200);
    lv_obj_set_pos(img_btn_confirm, 20, 246);
    lv_obj_add_event_cb(img_btn_confirm, done_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_confirm, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_confirm = lv_label_create(img_btn_confirm);
    lv_obj_set_style_text_color(label_confirm, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_confirm, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_confirm, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_confirm, LV_LABEL_LONG_DOT);
    lv_obj_set_width(label_confirm, 200);
    lv_label_set_text(label_confirm, "Done");
    lv_obj_center(label_confirm);
    lv_obj_add_flag(label_confirm, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t* img_btn_qrcode = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_qrcode, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
    lv_imagebutton_set_src(img_btn_qrcode, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
    lv_obj_set_width(img_btn_qrcode, 200);
    lv_obj_set_pos(img_btn_qrcode, 20, 278);
    lv_obj_add_event_cb(img_btn_qrcode, qrcode_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_qrcode, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_qrcode = lv_label_create(img_btn_qrcode);
    lv_obj_set_style_text_color(label_qrcode, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_qrcode, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(label_qrcode, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label_qrcode, LV_LABEL_LONG_DOT);
    lv_obj_set_width(label_qrcode, 200);
    lv_label_set_text(label_qrcode, "QR code");
    lv_obj_center(label_qrcode);
    lv_obj_add_flag(label_qrcode, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
    p_view_addr_main->label_qrcode = label_qrcode;
}


void view_addr_main_start(void)
{
    p_view_addr_main = (view_addr_main_t*)lv_malloc(sizeof(view_addr_main_t));
    LV_ASSERT(p_view_addr_main);
    lv_memset(p_view_addr_main, 0, sizeof(view_addr_main_t));

    p_view_addr_main->bg_cont = gui_comm_draw_bg();
    view_addr_main_bg_cont(p_view_addr_main->bg_cont);
}

 void view_addr_main_stop(void)
{
    lv_obj_del(p_view_addr_main->bg_cont);
    lv_free(p_view_addr_main);
    p_view_addr_main = NULL;
}
