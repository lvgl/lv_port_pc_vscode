/*********************
 *      INCLUDES
 *********************/
#ifndef __GUI_COMM_H__
#define __GUI_COMM_H__


#include <lvgl.h>
#include <stdio.h>


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


enum 
{	
	APP_STARTUP,
	APP_MAIN,
	APP_SETTINGS,
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

lv_obj_t* gui_comm_draw_bg(void);
void gui_comm_draw_keypad(lv_obj_t* parent, lv_event_cb_t num_cb, lv_event_cb_t ok_cb, lv_event_cb_t cancle_cb);
void gui_comm_draw_title(lv_obj_t *parent, const char *txt, lv_event_cb_t event_cb);

#endif /* __GUI_COMM_H__ */


