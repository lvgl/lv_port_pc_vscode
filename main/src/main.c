/**
 * @file main/src/main.c
 * 这里是主页面
 */

/* 包含图片数组头文件 */
/* #include "../assets/btn_img_up.h"
#include "../assets/btn_img_down.h" */

#define _DEFAULT_SOURCE /* usleep()需要此定义 */
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>  // 添加此行
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"

#include "gui_comm.h"
#include "gui_data_comm.h"

// 声明 tip.c 和 guide.c 中的函数
extern void load_tips_screen();
extern void add_tips_button_event(lv_obj_t * btn);
extern void load_guide_screen();
extern void add_guide_button_event(lv_obj_t * btn);

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  静态函数声明
 **********************/
static lv_display_t * hal_init(int32_t w, int32_t h);
static void settings_btn_event_handler(lv_event_t * e); // 声明事件处理函数
void load_main_screen(); // 声明加载主屏幕的函数
extern void load_settings_screen(); // 确保声明外部函数

/**********************
 *  静态变量
 **********************/

/**********************
 *      宏定义
 **********************/

/**********************
 *   全局函数
 **********************/

int main(int argc, char **argv)
{
    (void)argc; /* 未使用 */
    (void)argv; /* 未使用 */

    /* 初始化LVGL */
    lv_init();

    /* 初始化LVGL的HAL（显示，输入设备，计时） */
    hal_init(240, 320);

    /* 加载主屏幕 */
    load_main_screen();

    while(1) {
        /* 定期调用lv_task处理器 */
        lv_timer_handler();
        usleep(5 * 1000);
    }

    return 0;
}

/**********************
 *   静态函数
 **********************/

/**
 * 初始化LVGL图形库的硬件抽象层（HAL）
 */
static lv_display_t * hal_init(int32_t w, int32_t h)
{
    lv_group_set_default(lv_group_create());

    lv_display_t * disp = lv_sdl_window_create(w, h);

    lv_indev_t * mouse = lv_sdl_mouse_create();
    lv_indev_set_group(mouse, lv_group_get_default());
    lv_indev_set_display(mouse, disp);
    lv_display_set_default(disp);

    LV_IMAGE_DECLARE(mouse_cursor_icon); /* 声明图像文件。*/
    lv_obj_t * cursor_obj;
    cursor_obj = lv_image_create(lv_scr_act()); /* 创建一个用于光标的图像对象 */
    lv_image_set_src(cursor_obj, &mouse_cursor_icon); /* 设置图像源 */
    lv_indev_set_cursor(mouse, cursor_obj); /* 将图像对象连接到驱动程序 */

    lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
    lv_indev_set_display(mousewheel, disp);
    lv_indev_set_group(mousewheel, lv_group_get_default());

    lv_indev_t * kb = lv_sdl_keyboard_create();
    lv_indev_set_display(kb, disp);
    lv_indev_set_group(kb, lv_group_get_default());

    return disp;
}

/**
 * 加载主屏幕
 */
void load_main_screen() {
	gui_data_init();

//	extern void view_transaction_main_start(void);
//	view_transaction_main_start();
	extern void menu_main_start(void);
	menu_main_start();

//	extern void menu_main_start(void);
//	menu_main_start();
//	extern void view_transaction_main_start(void);
//	view_transaction_main_start();

	return;
}

