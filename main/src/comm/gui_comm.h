/*********************
 *      INCLUDES
 *********************/
#ifndef __GUI_COMM_H__
#define __GUI_COMM_H__

#include <string.h>
#include <lvgl.h>
#include <stdio.h>
#include <stdlib.h>


LV_IMG_DECLARE(img_backgroud);
LV_IMG_DECLARE(img_imgbtn_cancle_15x15);
LV_IMG_DECLARE(img_imgbtn_ok_12x9);
LV_IMG_DECLARE(img_left_pressed_8f7ff0_16x40);
LV_IMG_DECLARE(img_left_pressed_bbbbbb_14x26);
LV_IMG_DECLARE(img_left_pressed_bbbbbb_16x40);
LV_IMG_DECLARE(img_left_pressed_ff593c_16x40);
LV_IMG_DECLARE(img_left_released_6c6cf4_14x26);
LV_IMG_DECLARE(img_left_released_6c6cf4_16x40);
LV_IMG_DECLARE(img_left_released_6c6cf4_16x80);
LV_IMG_DECLARE(img_left_released_888888_14x26);
LV_IMG_DECLARE(img_left_released_888888_16x40);
LV_IMG_DECLARE(img_left_released_ff2600_16x40);
LV_IMG_DECLARE(img_menu_connect);
LV_IMG_DECLARE(img_menu_general);
LV_IMG_DECLARE(img_menu_guide);
LV_IMG_DECLARE(img_menu_settings);
LV_IMG_DECLARE(img_mid_pressed_8f7ff0_4x40);
LV_IMG_DECLARE(img_mid_pressed_bbbbbb_4x26);
LV_IMG_DECLARE(img_mid_pressed_bbbbbb_4x40);
LV_IMG_DECLARE(img_mid_pressed_ff593c_4x40);
LV_IMG_DECLARE(img_mid_released_6c6cf4_4x26);
LV_IMG_DECLARE(img_mid_released_6c6cf4_4x40);
LV_IMG_DECLARE(img_mid_released_6c6cf4_16x80);
LV_IMG_DECLARE(img_mid_released_888888_4x26);
LV_IMG_DECLARE(img_mid_released_888888_4x40);
LV_IMG_DECLARE(img_mid_released_ff2600_4x40);
LV_IMG_DECLARE(img_right_pressed_8f7ff0_16x40);
LV_IMG_DECLARE(img_right_pressed_bbbbbb_14x26);
LV_IMG_DECLARE(img_right_pressed_bbbbbb_16x40);
LV_IMG_DECLARE(img_right_pressed_ff593c_16x40);
LV_IMG_DECLARE(img_right_released_6c6cf4_14x26);
LV_IMG_DECLARE(img_right_released_6c6cf4_16x40);
LV_IMG_DECLARE(img_right_released_6c6cf4_16x80);
LV_IMG_DECLARE(img_right_released_888888_14x26);
LV_IMG_DECLARE(img_right_released_888888_16x40);
LV_IMG_DECLARE(img_right_released_ff2600_16x40);
LV_IMG_DECLARE(img_guide_back);
LV_IMG_DECLARE(img_screen_shape);
LV_IMG_DECLARE(img_slider_knob_1);
LV_IMG_DECLARE(img_slider_knob_2);
LV_IMG_DECLARE(img_slider_knob_3);
LV_IMG_DECLARE(img_slider_knob_4);
LV_IMG_DECLARE(img_success);
LV_IMG_DECLARE(img_unchosen);
LV_IMG_DECLARE(img_right_pressed_ffef4b_14x26);
LV_IMG_DECLARE(img_right_released_10e807_14x26);
LV_IMG_DECLARE(img_right_released_f7e100_14x26);
LV_IMG_DECLARE(img_mid_released_f7e100_4x26);
LV_IMG_DECLARE(img_right_pressed_61fc5a_14x26);
LV_IMG_DECLARE(img_mid_released_10e807_4x26);
LV_IMG_DECLARE(img_left_released_10e807_14x26);
LV_IMG_DECLARE(img_left_released_f7e100_14x26);
LV_IMG_DECLARE(img_mid_pressed_61fc5a_4x26);
LV_IMG_DECLARE(img_mid_pressed_ffef4b_4x26);
LV_IMG_DECLARE(img_keypad_back);
LV_IMG_DECLARE(img_keypad_confirm);
LV_IMG_DECLARE(img_left_pressed_61fc5a_14x26);
LV_IMG_DECLARE(img_left_pressed_ffef4b_14x26);
LV_IMG_DECLARE(img_chosen);
LV_IMG_DECLARE(img_alert);
LV_IMG_DECLARE(img_waiting);
LV_IMG_DECLARE(img_keypad_666666_q);
LV_IMG_DECLARE(img_keypad_666666_w);
LV_IMG_DECLARE(img_keypad_666666_e);
LV_IMG_DECLARE(img_keypad_666666_r);
LV_IMG_DECLARE(img_keypad_666666_t);
LV_IMG_DECLARE(img_keypad_666666_y);
LV_IMG_DECLARE(img_keypad_666666_u);
LV_IMG_DECLARE(img_keypad_666666_i);
LV_IMG_DECLARE(img_keypad_666666_o);
LV_IMG_DECLARE(img_keypad_666666_p);
LV_IMG_DECLARE(img_keypad_666666_a);
LV_IMG_DECLARE(img_keypad_666666_s);
LV_IMG_DECLARE(img_keypad_666666_d);
LV_IMG_DECLARE(img_keypad_666666_f);
LV_IMG_DECLARE(img_keypad_666666_g);
LV_IMG_DECLARE(img_keypad_666666_h);
LV_IMG_DECLARE(img_keypad_666666_j);
LV_IMG_DECLARE(img_keypad_666666_k);
LV_IMG_DECLARE(img_keypad_666666_l);
LV_IMG_DECLARE(img_keypad_666666_z);
LV_IMG_DECLARE(img_keypad_666666_x);
LV_IMG_DECLARE(img_keypad_666666_c);
LV_IMG_DECLARE(img_keypad_666666_v);
LV_IMG_DECLARE(img_keypad_666666_b);
LV_IMG_DECLARE(img_keypad_666666_n);
LV_IMG_DECLARE(img_keypad_666666_m);
LV_IMG_DECLARE(img_keypad_222222_q);
LV_IMG_DECLARE(img_keypad_222222_w);
LV_IMG_DECLARE(img_keypad_222222_e);
LV_IMG_DECLARE(img_keypad_222222_r);
LV_IMG_DECLARE(img_keypad_222222_t);
LV_IMG_DECLARE(img_keypad_222222_y);
LV_IMG_DECLARE(img_keypad_222222_u);
LV_IMG_DECLARE(img_keypad_222222_i);
LV_IMG_DECLARE(img_keypad_222222_o);
LV_IMG_DECLARE(img_keypad_222222_p);
LV_IMG_DECLARE(img_keypad_222222_a);
LV_IMG_DECLARE(img_keypad_222222_s);
LV_IMG_DECLARE(img_keypad_222222_d);
LV_IMG_DECLARE(img_keypad_222222_f);
LV_IMG_DECLARE(img_keypad_222222_g);
LV_IMG_DECLARE(img_keypad_222222_h);
LV_IMG_DECLARE(img_keypad_222222_j);
LV_IMG_DECLARE(img_keypad_222222_k);
LV_IMG_DECLARE(img_keypad_222222_l);
LV_IMG_DECLARE(img_keypad_222222_z);
LV_IMG_DECLARE(img_keypad_222222_x);
LV_IMG_DECLARE(img_keypad_222222_c);
LV_IMG_DECLARE(img_keypad_222222_v);
LV_IMG_DECLARE(img_keypad_222222_b);
LV_IMG_DECLARE(img_keypad_222222_n);
LV_IMG_DECLARE(img_keypad_222222_m);
LV_IMG_DECLARE(img_startup_arrow);
LV_IMG_DECLARE(img_screen_shoosen);
LV_IMG_DECLARE(img_bg1);
LV_IMG_DECLARE(img_bg2);
LV_IMG_DECLARE(img_bg3);
LV_IMG_DECLARE(img_bg4);
LV_IMG_DECLARE(img_bg5);
LV_IMG_DECLARE(img_bg6);
LV_IMG_DECLARE(img_bg7);
LV_IMG_DECLARE(img_bg1_small);
LV_IMG_DECLARE(img_bg2_small);
LV_IMG_DECLARE(img_bg3_small);
LV_IMG_DECLARE(img_bg4_small);
LV_IMG_DECLARE(img_bg5_small);
LV_IMG_DECLARE(img_bg6_small);
LV_IMG_DECLARE(img_bg7_small);
LV_IMG_DECLARE(img_name);



typedef void (*gui_comm_callback)(void);


enum 
{	
	APP_STARTUP,
	APP_STARTUP_CREATE_WALLET,
	APP_STARTUP_IMPORT_WALLET,
	APP_MAIN,
	APP_SETTINGS,
	APP_SETTINGS_PIN,
	APP_SETTINGS_ERASE,
	APP_GENERAL,
	APP_CONNECT,
	APP_GUIDE,
};
typedef uint8_t app_index_t;

typedef struct
{
    const char *str;
    lv_coord_t x;
    lv_coord_t y;
} gui_comm_imgbtn_desc_t;

typedef struct
{
    const char* str;
    lv_coord_t x;
    lv_coord_t y;
    const void* src;
} gui_comm_img_desc_t;

typedef struct
{
    const char* str;
    lv_coord_t x;
    lv_coord_t y;
    const void* src_pressed;
    const void* src_released;
} gui_comm_keypad_desc_t;

typedef struct
{
    lv_coord_t x;
    gui_comm_callback cb;
} gui_comm_slider_t;

lv_obj_t* gui_comm_draw_bg(void);
void gui_comm_draw_keypad_letter(lv_obj_t* parent, lv_event_cb_t num_cb, lv_event_cb_t ok_cb, lv_event_cb_t cancle_cb);
void gui_comm_draw_keypad_num(lv_obj_t* parent, lv_event_cb_t num_cb, lv_event_cb_t ok_cb, lv_event_cb_t cancle_cb);
void gui_comm_draw_title(lv_obj_t *parent, const char *txt, lv_event_cb_t event_cb);
lv_obj_t * gui_comm_draw_slider(lv_obj_t* parent, lv_coord_t y, gui_comm_callback cb);

#endif /* __GUI_COMM_H__ */


