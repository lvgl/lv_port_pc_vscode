/**
 * @file main/src/wallet.c
 *
 */


#include "lvgl/lvgl.h"
#include <unistd.h>  // 添加这个头文件以使用 usleep

LV_IMG_DECLARE(mouse_cursor_icon); // 确保声明 mouse_cursor_icon

static lv_display_t * hal_init(int32_t w, int32_t h);

void create_button(lv_obj_t * parent, const char * text, lv_coord_t y_offset, lv_color_t text_color);

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
    lv_label_set_text(label, "Cold Wallet");
    lv_obj_set_style_text_color(label, lv_color_white(), 0);  // 设置标题文字颜色为白色
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);  // 将标题居中放置在顶部

    /* 创建多个按钮 */
    create_button(container, "Import Wallet", -30, lv_color_black());
    create_button(container, "Create Wallet", 30, lv_color_black());

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

/**
 * 创建按钮函数
 */
void create_button(lv_obj_t * parent, const char * text, lv_coord_t y_offset, lv_color_t text_color) {
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 200, 30);  // 设置按钮的宽度为200，高度为30
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, y_offset);  // 将按钮放置在容器中央偏下，与上一个按钮有间隙
    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 10);  // 设置按钮的圆角
    lv_style_set_bg_color(&style_btn, lv_color_hex(0xD8D8D8));  // 设置按钮的背景颜色为0xD8D8D8
    lv_obj_add_style(btn, &style_btn, 0);

    lv_obj_t * btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, text);
    lv_obj_set_style_text_color(btn_label, text_color, 0);  // 设置按钮文字颜色
    lv_obj_center(btn_label);  // 确保文字在按钮中居中对齐
}
