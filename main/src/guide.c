/**
 * @file main/src/guide.c
 *
 */
#include "lvgl/lvgl.h"
#include <string.h>

// 声明加载主屏幕的函数，以便在返回按钮事件处理程序中使用
extern void load_main_screen();

// 前向声明 load_guide_screen 函数
void load_guide_screen();

// 函数声明
lv_obj_t * create_guide_button(lv_obj_t * parent, const char * text, lv_coord_t y_offset, lv_color_t text_color);

/**
 * Guide按钮点击事件处理函数
 */
static void guide_btn_event_handler(lv_event_t * e)
{
    load_guide_screen(); // 调用函数加载 guide 屏幕
}

/**
 * 返回按钮点击事件处理函数
 */
static void back_btn_event_handler(lv_event_t * e)
{
    load_main_screen(); // 调用函数返回主屏幕
}

/**
 * 加载Guide页面
 */
void load_guide_screen() {
    /* 创建Guide页面 */
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
    lv_label_set_text(label, "Guide");
    lv_obj_set_style_text_color(label, lv_color_white(), 0);  // 设置标题文字颜色为白色
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);  // 将标题居中放置在顶部

    /* 创建多个按钮 */
    create_guide_button(container, "Step 1", -90, lv_color_black());
    create_guide_button(container, "Step 2", -50, lv_color_black());
    create_guide_button(container, "Step 3", -10, lv_color_black());
    create_guide_button(container, "Step 4", 30, lv_color_black());
    create_guide_button(container, "Step 5", 70, lv_color_black());

    /* 创建Back按钮 */
    lv_obj_t * back_btn = create_guide_button(container, "Back", 150, lv_color_black());
    lv_obj_add_event_cb(back_btn, back_btn_event_handler, LV_EVENT_CLICKED, NULL);
}

/**
 * 创建按钮函数
 */
lv_obj_t * create_guide_button(lv_obj_t * parent, const char * text, lv_coord_t y_offset, lv_color_t text_color) {
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
 * 在主屏幕中调用此函数设置 Guide 按钮的事件处理程序
 */
void add_guide_button_event(lv_obj_t * btn)
{
    lv_obj_add_event_cb(btn, guide_btn_event_handler, LV_EVENT_CLICKED, NULL);
}
