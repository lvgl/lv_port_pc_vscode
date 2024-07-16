/**
 * @file main/src/tip.c
 *
 */
#include "lvgl/lvgl.h"
#include <string.h>

// 声明加载主屏幕的函数，以便在返回按钮事件处理程序中使用
extern void load_main_screen();

// 前向声明 load_tips_screen 函数
void load_tips_screen();

/**
 * Tips按钮点击事件处理函数
 */
static void tips_btn_event_handler(lv_event_t * e)
{
    load_tips_screen(); // 调用函数加载 tips 屏幕
}

/**
 * 返回按钮点击事件处理函数
 */
static void back_btn_event_handler(lv_event_t * e)
{
    load_main_screen(); // 调用函数返回主屏幕
}

/**
 * 加载Tips页面
 */
void load_tips_screen() {
    /* 创建Tips页面 */
    lv_obj_t * scr = lv_scr_act();

    /* 清空当前屏幕 */
    lv_obj_clean(scr);

    /* 创建返回按钮 */
    lv_obj_t * back_btn = lv_btn_create(scr);
    lv_obj_set_size(back_btn, 100, 50);
    lv_obj_align(back_btn, LV_ALIGN_BOTTOM_MID, 0, -10);  // 按钮位置为屏幕底部中间

    lv_obj_t * label = lv_label_create(back_btn);
    lv_label_set_text(label, "Back");
    lv_obj_center(label);

    lv_obj_add_event_cb(back_btn, back_btn_event_handler, LV_EVENT_CLICKED, NULL);
}

/**
 * 在主屏幕中调用此函数设置 Tips 按钮的事件处理程序
 */
void add_tips_button_event(lv_obj_t * btn)
{
    lv_obj_add_event_cb(btn, tips_btn_event_handler, LV_EVENT_CLICKED, NULL);
}
