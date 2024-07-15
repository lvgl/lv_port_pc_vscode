/**
 * @file settings.c
 *
 */

#include "lvgl/lvgl.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>  // 包含字符串操作的头文件

/* 静态函数声明 */
static lv_display_t * hal_init(int32_t w, int32_t h);
static void create_settings_screen(void);
static void settings_btn_event_cb(lv_event_t * e);

/**
 * 主函数
 */
int main(int argc, char **argv)
{
    (void)argc; /* 未使用 */
    (void)argv; /* 未使用 */

    /* 初始化LVGL */
    lv_init();

    /* 初始化LVGL的HAL（显示，输入设备，计时） */
    lv_display_t * disp = hal_init(480, 272);

    /* 调用 Settings 页面 */
    create_settings_screen();

    while(1) {
        /* 定期调用lv_task处理器。
         * 这也可以在定时器中断或操作系统任务中完成。*/
        lv_timer_handler();
        usleep(5 * 1000);
    }

    return 0;
}

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
 * 创建 Settings 页面
 */
static void create_settings_screen(void)
{
    /* 获取当前活动的屏幕 */
    lv_obj_t * scr = lv_scr_act();

    /* 创建一个容器来放置 Settings */
    lv_obj_t * cont = lv_obj_create(scr);
    lv_obj_set_size(cont, 240, 320); // 设置尺寸为 240x320
    lv_obj_center(cont);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x000000), 0);  // 黑色背景
    lv_obj_set_style_radius(cont, 10, 0);  // 圆角

    /* 创建一个标题标签来显示 "Settings" */
    lv_obj_t * title_label = lv_label_create(cont);
    lv_label_set_text(title_label, "Settings");
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_14, 0);  // 使用已经声明的字体
    lv_obj_set_style_text_color(title_label, lv_color_hex(0xFFFFFF), 0); // 设置文字颜色为白色
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);

    /* 创建按钮数组 */
    const char * btn_labels[] = {"Home Screen", "Security", "Language", "About Device", "Brightness", "Power Off"};
    for(int i = 0; i < 6; i++) {
        lv_obj_t * btn = lv_btn_create(cont);
        lv_obj_set_size(btn, 200, 40);
        lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 50 + i * 50);
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, btn_labels[i]);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
        lv_obj_center(label);
        lv_obj_add_event_cb(btn, settings_btn_event_cb, LV_EVENT_CLICKED, (void *)btn_labels[i]); // 添加事件回调
    }
}

/**
 * 按钮事件回调
 */
static void settings_btn_event_cb(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);
    const char * txt = lv_label_get_text(lv_obj_get_child(btn, 0));
    LV_LOG_USER("%s button clicked", txt);
    // 根据需要添加按钮点击处理代码
}
