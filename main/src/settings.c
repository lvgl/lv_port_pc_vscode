#include "lvgl/lvgl.h"
#include <unistd.h>
#include <stdio.h>  // 添加此行以包含 printf 函数
#include <string.h> // 添加此行以包含 strcmp 函数

LV_IMG_DECLARE(mouse_cursor_icon); // 确保声明 mouse_cursor_icon

static lv_display_t * hal_init(int32_t w, int32_t h);
static void back_btn_event_handler(lv_event_t * e); // 声明Back按钮事件处理函数
static void about_device_btn_event_handler(lv_event_t * e); // 声明关于设备按钮事件处理函数
static void reset_device_btn_event_handler(lv_event_t * e); // 声明重置设备按钮事件处理函数
static void msgbox_btn_event_handler(lv_event_t * e); // 声明消息框按钮事件处理函数

lv_obj_t * create_button(lv_obj_t * parent, const char * text, lv_coord_t y_offset, lv_color_t text_color);

/**
 * 显示重置弹窗
 */
static void show_reset_popup() {
    lv_obj_t * popup = lv_obj_create(lv_scr_act());
    lv_obj_set_size(popup, 200, 150);
    lv_obj_center(popup);

    lv_obj_t * title = lv_label_create(popup);
    lv_label_set_text(title, "Reset Device");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t * msg = lv_label_create(popup);
    lv_label_set_text(msg, "Are you sure you want to reset the device?");
    lv_obj_align(msg, LV_ALIGN_CENTER, 0, -10);

    static const char * btns[] = {"Continue", "Cancel", ""};
    lv_obj_t * btnm = lv_btnmatrix_create(popup);
    lv_btnmatrix_set_map(btnm, btns);
    lv_obj_align(btnm, LV_ALIGN_BOTTOM_MID, 0, -10);

    lv_obj_add_event_cb(btnm, msgbox_btn_event_handler, LV_EVENT_VALUE_CHANGED, popup);
}

static void msgbox_btn_event_handler(lv_event_t * e) {
    lv_obj_t * btnm = lv_event_get_target(e);
    const char * txt = lv_btnmatrix_get_btn_text(btnm, lv_btnmatrix_get_selected_btn(btnm));
    lv_obj_t * popup = lv_event_get_user_data(e);

    if(strcmp(txt, "Continue") == 0) {
        // 在此处添加重置设备的逻辑
        printf("Device is being reset...\n");
    }
    else if(strcmp(txt, "Cancel") == 0) {
        printf("Reset cancelled.\n");
    }

    lv_obj_del(popup);  // 关闭消息框
}

static void reset_device_btn_event_handler(lv_event_t * e) {
    show_reset_popup();  // 显示重置弹窗
}

/**
 * 加载 settings 屏幕
 */
void load_settings_screen() {
    /* 创建主屏幕 */
    lv_obj_t * scr = lv_scr_act();

    /* 清空当前屏幕 */
    lv_obj_clean(scr);

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

    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 10);  // 设置按钮的圆角
    lv_style_set_bg_color(&style_btn, lv_color_hex(0xD8D8D8));  // 设置按钮的背景颜色为0xD8D8D8

    /* 创建多个按钮 */
    create_button(container, "Wallpaper", -90, lv_color_black());
    create_button(container, "Language", -50, lv_color_black());
    create_button(container, "Brightness", -10, lv_color_black());
    lv_obj_t * about_btn = create_button(container, "About Device", 30, lv_color_black());
    lv_obj_add_event_cb(about_btn, about_device_btn_event_handler, LV_EVENT_CLICKED, NULL); // 添加关于设备按钮的事件处理
    create_button(container, "System Update", 70, lv_color_black());
    lv_obj_t * reset_btn = create_button(container, "Reset Device", 110, lv_color_black());
    lv_obj_add_event_cb(reset_btn, reset_device_btn_event_handler, LV_EVENT_CLICKED, NULL);  // 添加重置设备按钮的事件处理
    create_button(container, "Power Off", 150, lv_color_hex(0xFF0000));  

    /* 创建Back按钮 */
    lv_obj_t * back_btn = create_button(container, "Back", 190, lv_color_black());
    lv_obj_add_event_cb(back_btn, back_btn_event_handler, LV_EVENT_CLICKED, NULL);
}

/**
 * 加载关于设备页面
 */
void load_about_device_screen() {
    /* 创建主屏幕 */
    lv_obj_t * scr = lv_scr_act();

    /* 清空当前屏幕 */
    lv_obj_clean(scr);

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
    lv_label_set_text(label, "About Device");
    lv_obj_set_style_text_color(label, lv_color_white(), 0);  // 设置标题文字颜色为白色
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);  // 将标题居中放置在顶部

    /* 创建设备信息的标签 */
    lv_obj_t * info_label = lv_label_create(container);
    lv_label_set_text(info_label, "Device Name: My Device\nVersion: 1.0\nManufacturer: My Company");
    lv_obj_set_style_text_color(info_label, lv_color_white(), 0);  // 设置文字颜色为白色
    lv_obj_align(info_label, LV_ALIGN_CENTER, 0, 0);  // 将标签居中放置

    /* 创建Back按钮 */
    lv_obj_t * back_btn = create_button(container, "Back", 190, lv_color_black());
    lv_obj_add_event_cb(back_btn, back_btn_event_handler, LV_EVENT_CLICKED, NULL);
}

/**
 * 关于设备按钮点击事件处理函数
 */
static void about_device_btn_event_handler(lv_event_t * e) {
    load_about_device_screen(); // 加载关于设备页面
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
lv_obj_t * create_button(lv_obj_t * parent, const char * text, lv_coord_t y_offset, lv_color_t text_color) {
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
    
    return btn;
}

/**
 * Back按钮点击事件处理函数
 */
static void back_btn_event_handler(lv_event_t * e) {
    extern void load_main_screen(); // 声明外部函数
    load_main_screen(); // 调用函数加载 main 屏幕
}
