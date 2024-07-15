// src/settings.c

#include "lvgl/lvgl.h"

static lv_display_t * hal_init(int32_t w, int32_t h);

int main(int argc, char **argv) {
    (void)argc; /* 未使用 */
    (void)argv; /* 未使用 */

    /* 初始化LVGL */
    lv_init();

    /* 初始化LVGL的HAL（显示，输入设备，计时） */
    hal_init(480, 272);

    /* 创建主屏幕 */
    lv_obj_t * scr = lv_scr_act();

    /* 创建一个新的容器作为页面 */
    lv_obj_t * container = lv_obj_create(scr);
    lv_obj_set_size(container, 240, 320);
    lv_obj_center(container);

    /* 创建容器的样式并设置背景为黑色 */
    static lv_style_t style_container;
    lv_style_init(&style_container);
    lv_style_set_bg_color(&style_container, lv_color_black());
    lv_obj_add_style(container, &style_container, 0);

    /* 创建一个标签作为标题 */
    lv_obj_t * label = lv_label_create(container);
    lv_label_set_text(label, "Settings");
    lv_obj_set_style_text_color(label, lv_color_white(), 0);  // 设置标题文字颜色为白色
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);  // 将标题居中放置在顶部

    while(1) {
        /* 定期调用lv_task处理器 */
        lv_timer_handler();
        usleep(5 * 1000);
    }

    return 0;
}

/**
 * 初始化LVGL图形库的硬件抽象层（HAL）
 */
static lv_display_t * hal_init(int32_t w, int32_t h) {
    lv_group_set_default(lv_group_create());

    lv_display_t * disp = lv_sdl_window_create(w, h);

    lv_indev_t * mouse = lv_sdl_mouse_create();
    lv_indev_set_group(mouse, lv_group_get_default());
    lv_indev_set_display(mouse, disp);
    lv_display_set_default(disp);

    LV_IMAGE_DECLARE(mouse_cursor_icon); /* 声明图像文件 */
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
