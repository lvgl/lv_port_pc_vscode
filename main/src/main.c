/**
 * @file main/src/main.c
 *
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
    hal_init(480, 272);

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
    cursor_obj = lv_image_create(lv_screen_active()); /* 创建一个用于光标的图像对象 */
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
    /* 创建主屏幕 */
    lv_obj_t * scr = lv_scr_act();

    /* 清空当前屏幕 */
    lv_obj_clean(scr);

    /* 创建一个用于放置方块的网格 */
    static lv_coord_t col_dsc[] = {80, LV_GRID_FR(1), 80, LV_GRID_TEMPLATE_LAST};  // 每个方块80px，中间间隙均分
    static lv_coord_t row_dsc[] = {70, LV_GRID_FR(1), 70, LV_GRID_FR(1), 70, LV_GRID_TEMPLATE_LAST};  // 每个方块70px，中间间隙均分

    lv_obj_t * grid = lv_obj_create(scr);
    lv_obj_set_size(grid, 240, 320);  // 宽度为240px，高度为320px
    lv_obj_center(grid);
    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);

    /* 创建容器的样式并设置背景为黑色 */
    static lv_style_t style_container;
    lv_style_init(&style_container);
    lv_style_set_bg_color(&style_container, lv_color_black());  /* 设置背景颜色为黑色 */
    lv_obj_add_style(grid, &style_container, 0);  /* 将样式应用到容器上 */

    /* 创建方块的样式 */
    static lv_style_t style_block;
    lv_style_init(&style_block);
    lv_style_set_bg_color(&style_block, lv_color_hex(0xD8D8D8));  /* 灰色背景 */
    lv_style_set_radius(&style_block, 10);  /* 圆角 */

    /* 更新按钮文本数组以移除不需要的按钮 */
    const char *btn_texts[3][2] = {
        {"Settings", "Tips"},
        {"Guide", NULL},  // 将 "Button 4" 更改为 NULL
        {NULL, NULL}      // 将 "Button 5" 和 "Button 6" 更改为 NULL
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            if (btn_texts[i][j] != NULL) {  // 只有当存在有效文本时才创建按钮
                lv_obj_t * block = lv_btn_create(grid);
                lv_obj_set_size(block, 80, 70);  // 设置宽度为80px，高度为70px
                lv_obj_add_style(block, &style_block, 0);
                lv_obj_set_grid_cell(block, LV_GRID_ALIGN_CENTER, j * 2, 1, LV_GRID_ALIGN_CENTER, i * 2, 1);

                /* 创建标签并设置样式 */
                lv_obj_t * label = lv_label_create(block);
                lv_label_set_text(label, btn_texts[i][j]);
                lv_obj_set_style_text_color(label, lv_color_black(), 0);  // 设置文字颜色为黑色
                lv_obj_center(label);  // 使文字在按钮中居中

                /* 如果是Settings按钮，添加事件处理 */
                if (strcmp(btn_texts[i][j], "Settings") == 0) {
                    lv_obj_add_event_cb(block, settings_btn_event_handler, LV_EVENT_CLICKED, NULL);
                }
            }
        }
    }
}

/**
 * 设置按钮点击事件处理函数
 */
static void settings_btn_event_handler(lv_event_t * e)
{
    load_settings_screen(); // 调用函数加载 settings 屏幕
}
