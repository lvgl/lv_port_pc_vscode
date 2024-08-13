/*********************
 *      INCLUDES
 *********************/

#include "gui_comm.h"


static gui_comm_keypad_desc_t gui_comm_keypad_desc_table[] =
{
	{"back", 11, 280, &img_keypad_back, &img_keypad_back},
	{"confirm", 198, 280, &img_keypad_confirm, &img_keypad_confirm},
	{"q", 11, 222, &img_keypad_666666_q, &img_keypad_222222_q},
	{"w", 33, 222, &img_keypad_666666_w, &img_keypad_222222_w},
	{"e", 55, 222, &img_keypad_666666_e, &img_keypad_222222_e},
	{"r", 77, 222, &img_keypad_666666_r, &img_keypad_222222_r},
	{"t", 99, 222, &img_keypad_666666_t, &img_keypad_222222_t},
	{"y", 121, 222, &img_keypad_666666_y, &img_keypad_222222_y},
	{"u", 143, 222, &img_keypad_666666_u, &img_keypad_222222_u},
	{"i", 165, 222, &img_keypad_666666_i, &img_keypad_222222_i},
	{"o", 187, 222, &img_keypad_666666_o, &img_keypad_222222_o},
	{"p", 209, 222, &img_keypad_666666_p, &img_keypad_222222_p},
	{"a", 22, 248, &img_keypad_666666_a, &img_keypad_222222_a},
	{"s", 44, 248, &img_keypad_666666_s, &img_keypad_222222_s},
	{"d", 66, 248, &img_keypad_666666_d, &img_keypad_222222_d},
	{"f", 88, 248, &img_keypad_666666_f, &img_keypad_222222_f},
	{"g", 110, 248, &img_keypad_666666_g, &img_keypad_222222_g},
	{"h", 132, 248, &img_keypad_666666_h, &img_keypad_222222_h},
	{"j", 154, 248, &img_keypad_666666_j, &img_keypad_222222_j},
	{"k", 176, 248, &img_keypad_666666_k, &img_keypad_222222_k},
	{"l", 198, 248, &img_keypad_666666_l, &img_keypad_222222_l},
	{"z", 44, 280, &img_keypad_666666_z, &img_keypad_222222_z},
	{"x", 66, 280, &img_keypad_666666_x, &img_keypad_222222_x},
	{"c", 88, 280, &img_keypad_666666_c, &img_keypad_222222_c},
	{"v", 110, 280, &img_keypad_666666_v, &img_keypad_222222_v},
	{"b", 132, 280, &img_keypad_666666_b, &img_keypad_222222_b},
	{"n", 154, 280, &img_keypad_666666_n, &img_keypad_222222_n},
	{"m", 176, 280, &img_keypad_666666_m, &img_keypad_222222_m},
	
};


static gui_comm_imgbtn_desc_t gui_imgbtn_num_table[] =
{
    {"1", 20, 132},
    {"2", 88, 132},
    {"3", 156, 132},
    {"4", 20, 176},
    {"5", 88, 176},
    {"6", 156, 176},
    {"7", 20, 220},
    {"8", 88, 220},
    {"9", 156, 220},
    {"0", 88, 264},
};


void gui_comm_draw_keypad_letter(lv_obj_t* parent, lv_event_cb_t num_cb, lv_event_cb_t ok_cb, lv_event_cb_t cancle_cb)
{
    for (uint8_t i = 2; i < sizeof(gui_comm_keypad_desc_table) / sizeof(gui_comm_keypad_desc_t); i++)
    {
        lv_obj_t* img = lv_img_create(parent);
        lv_img_set_src(img, gui_comm_keypad_desc_table[i].src_released);
        lv_obj_set_pos(img, gui_comm_keypad_desc_table[i].x, gui_comm_keypad_desc_table[i].y);
        lv_obj_add_event_cb(img, num_cb, LV_EVENT_ALL, (void *)&gui_comm_keypad_desc_table[i]);
        lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
    }
    
    lv_obj_t* img_cancle = lv_img_create(parent);
    lv_img_set_src(img_cancle, gui_comm_keypad_desc_table[0].src_released);
    lv_obj_set_pos(img_cancle, gui_comm_keypad_desc_table[0].x, gui_comm_keypad_desc_table[0].y);
    lv_obj_add_event_cb(img_cancle, cancle_cb, LV_EVENT_ALL, (void *)&gui_comm_keypad_desc_table[0]);
    lv_obj_add_flag(img_cancle, LV_OBJ_FLAG_CLICKABLE);
    
    lv_obj_t* img_ok = lv_img_create(parent);
    lv_img_set_src(img_ok, gui_comm_keypad_desc_table[1].src_released);
    lv_obj_set_pos(img_ok, gui_comm_keypad_desc_table[1].x, gui_comm_keypad_desc_table[1].y);
    lv_obj_add_event_cb(img_ok, ok_cb, LV_EVENT_ALL, (void *)&gui_comm_keypad_desc_table[1]);
    lv_obj_add_flag(img_ok, LV_OBJ_FLAG_CLICKABLE);
}

void gui_comm_draw_keypad_num(lv_obj_t* parent, lv_event_cb_t num_cb, lv_event_cb_t ok_cb, lv_event_cb_t cancle_cb)
{
    for (uint8_t i = 0; i < sizeof(gui_imgbtn_num_table) / sizeof(gui_comm_imgbtn_desc_t); i++)
    {
        lv_obj_t* img_btn = lv_imagebutton_create(parent);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_16x40, &img_mid_released_888888_4x40, &img_right_released_888888_16x40);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_16x40, &img_mid_pressed_bbbbbb_4x40, &img_right_pressed_bbbbbb_16x40);
        lv_obj_set_width(img_btn, 64);
        lv_obj_set_pos(img_btn, gui_imgbtn_num_table[i].x, gui_imgbtn_num_table[i].y);
        lv_obj_add_event_cb(img_btn, num_cb, LV_EVENT_SHORT_CLICKED, (void *)gui_imgbtn_num_table[i].str);
        lv_obj_add_flag(img_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t *label = lv_label_create(img_btn);
        lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
        lv_label_set_text(label, gui_imgbtn_num_table[i].str);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    }

    lv_obj_t* img_btn_ok = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_ok, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_6c6cf4_16x40, &img_mid_released_6c6cf4_4x40, &img_right_released_6c6cf4_16x40);
    lv_imagebutton_set_src(img_btn_ok, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_8f7ff0_16x40, &img_mid_pressed_8f7ff0_4x40, &img_right_pressed_8f7ff0_16x40);
    lv_obj_set_width(img_btn_ok, 64);
    lv_obj_set_pos(img_btn_ok, 156, 264);
    lv_obj_add_event_cb(img_btn_ok, ok_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_ok, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *img_ok = lv_img_create(img_btn_ok);
    lv_img_set_src(img_ok, &img_imgbtn_ok_12x9);
    lv_obj_center(img_ok);

    lv_obj_t* img_btn_cancle = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_cancle, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_ff2600_16x40, &img_mid_released_ff2600_4x40, &img_right_released_ff2600_16x40);
    lv_imagebutton_set_src(img_btn_cancle, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_ff593c_16x40, &img_mid_pressed_ff593c_4x40, &img_right_pressed_ff593c_16x40);
    lv_obj_set_width(img_btn_cancle, 64);
    lv_obj_set_pos(img_btn_cancle, 20, 264);
    lv_obj_add_event_cb(img_btn_cancle, cancle_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_cancle, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* img_canle = lv_img_create(img_btn_cancle);
    lv_img_set_src(img_canle, &img_imgbtn_cancle_15x15);
    lv_obj_center(img_canle);
}


static void slider_knob_cb(lv_event_t* e)
{
	lv_obj_t *knob = lv_event_get_target(e);
    lv_event_code_t event = lv_event_get_code(e);
    gui_comm_slider_t *slider_data = e->user_data;

    if (LV_EVENT_PRESSING == event)
    {
    	if(NULL == slider_data->cb) return;
    	
    	lv_point_t point;
    	lv_indev_t *indev = lv_indev_get_act();
    	lv_indev_get_vect(indev, &point);
    	
    	slider_data->x += point.x;

    	if(slider_data->x < 25)
    		slider_data->x = 25;
    	else if(slider_data->x >= 185)
    	{
    		slider_data->x = 185;
    		lv_img_set_src(knob, &img_slider_knob_3);
    		lv_obj_t* label = knob->user_data;
			lv_label_set_text(label, "Processing...");
			slider_data->cb();
			slider_data->cb = NULL;
    	}
		lv_obj_set_x(knob, slider_data->x);
    	
    }
    else if (LV_EVENT_PRESSED == event)
    {
    	lv_img_set_src(knob, &img_slider_knob_2);
    }
    else if (LV_EVENT_RELEASED == event)
    {
    	if(slider_data->x != 185)
    	{
    		slider_data->x = 25;
			lv_obj_set_x(knob, slider_data->x);
    		lv_img_set_src(knob, &img_slider_knob_1);
    	}
    }
    else if (LV_EVENT_DELETE == event)
    {
    	printf("%s LV_EVENT_DELETE\n", __func__);
    	lv_free(e->user_data);
    }
}

lv_obj_t * gui_comm_draw_slider(lv_obj_t* parent, lv_coord_t y, gui_comm_callback cb)
{
	gui_comm_slider_t *slider_data = lv_malloc(sizeof(gui_comm_slider_t));
	lv_memset(slider_data, 0, sizeof(gui_comm_slider_t));
	slider_data->x = 25;
	slider_data->cb = cb;
	
    lv_obj_t *bg_obj = lv_obj_create(parent);
    lv_obj_set_style_bg_color(bg_obj, lv_color_hex(0x4B525B), LV_PART_MAIN);
    lv_obj_set_style_radius(bg_obj, 20, LV_PART_MAIN);
    lv_obj_set_size(bg_obj, 200, 40);
    lv_obj_set_pos(bg_obj, 20, y);

    lv_obj_t* label= lv_label_create(bg_obj);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_label_set_text(label, "Slide to Reset");
	lv_obj_center(label);

    lv_obj_t* img_knob = lv_img_create(parent);
	lv_img_set_src(img_knob, &img_slider_knob_1);
    lv_obj_set_pos(img_knob, slider_data->x, y + 5);
    lv_obj_add_event_cb(img_knob, slider_knob_cb, LV_EVENT_ALL, slider_data);
    lv_obj_add_flag(img_knob, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_user_data(img_knob, label);

	return img_knob;
}

lv_obj_t *gui_comm_draw_bg(void)
{
    lv_obj_t *bg_obj = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(bg_obj);
	lv_obj_set_scrollbar_mode(bg_obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_remove_flag(bg_obj, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_set_style_bg_color(bg_obj, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_size(bg_obj, 240, 320);
    lv_obj_center(bg_obj);
	lv_obj_update_layout(bg_obj);
	
    return bg_obj;
}

void gui_comm_draw_title(lv_obj_t *parent, const char *txt, lv_event_cb_t event_cb)
{
    if (event_cb)
    {
        lv_obj_t* back_icons = lv_img_create(parent);
        lv_img_set_src(back_icons, &img_guide_back);
        lv_obj_set_pos(back_icons, 20, 8);
        lv_obj_add_event_cb(back_icons, event_cb, LV_EVENT_SHORT_CLICKED, NULL);
        lv_obj_add_flag(back_icons, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
    }

    lv_obj_t *title = lv_label_create(parent);
    lv_obj_set_style_text_color(title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_label_set_long_mode(title, LV_LABEL_LONG_DOT);
    lv_obj_set_width(title, 200);
    lv_obj_set_pos(title, 20, 30);
    lv_label_set_text(title, txt);
    lv_obj_add_flag(title, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
}

