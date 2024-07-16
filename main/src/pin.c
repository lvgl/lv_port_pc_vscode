/**
 * @file main/src/pin.c
 *
 */


#include "lvgl/lvgl.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>  // 包含字符串操作的头文件

/* 静态函数声明 */
static lv_display_t * hal_init(int32_t w, int32_t h);
static void create_pin_code_screen(void);
static void pin_btn_event_cb(lv_event_t * e);

/* 全局变量 */
static lv_obj_t * pin_label;
static char pin_code[7] = "";  // 最大6位数字的PIN码

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

    /* 调用 PIN 码输入 demo */
    create_pin_code_screen();

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
 * 创建 PIN 码输入界面
 */
static void create_pin_code_screen(void)
{
    /* 获取当前活动的屏幕 */
    lv_obj_t * scr = lv_scr_act();

    /* 创建一个容器来放置 PIN 码输入 */
    lv_obj_t * cont = lv_obj_create(scr);
    lv_obj_set_size(cont, 240, 320);
    lv_obj_center(cont);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x000000), 0);  // 黑色背景
    lv_obj_set_style_radius(cont, 10, 0);  // 圆角

    /* 创建一个标题标签来显示 "Enter New Pin" */
    lv_obj_t * title_label = lv_label_create(cont);
    lv_label_set_text(title_label, "Enter New Pin");
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_14, 0);  // 使用已经声明的字体
    lv_obj_set_style_text_color(title_label, lv_color_hex(0xFFFFFF), 0); // 设置文字颜色为白色
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);

    /* 创建一个标签来显示输入的PIN码 */
    pin_label = lv_label_create(cont);
    lv_label_set_text(pin_label, "");
    lv_obj_set_style_text_font(pin_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(pin_label, lv_color_hex(0xFFFFFF), 0); // 设置文字颜色为白色
    lv_obj_align(pin_label, LV_ALIGN_TOP_MID, 0, 40);  // 修改Y坐标以避免与标题重叠

    /* 创建数字键盘 */
    static const char * btn_map[] = {
        "1", "2", "3", "\n",
        "4", "5", "6", "\n",
        "7", "8", "9", "\n",
        "DEL", "0", "OK", ""
    };

    lv_obj_t * btnm = lv_btnmatrix_create(cont);
    lv_btnmatrix_set_map(btnm, btn_map);
    lv_obj_set_size(btnm, 220, 180);
    lv_obj_align(btnm, LV_ALIGN_CENTER, 0, 90);  // 修改Y坐标以适应新布局

    /* 设置按钮背景颜色为D8D8D8并且去掉投影 */
    lv_obj_set_style_bg_color(btnm, lv_color_hex(0xD8D8D8), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(btnm, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);

    /* 设置数字键盘的背景颜色为黑色 */
    lv_obj_set_style_bg_color(btnm, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 设置数字键盘的边框颜色为黑色并去掉边框宽度 */
    lv_obj_set_style_border_color(btnm, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(btnm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 设置数字键盘按键文字颜色 */
    /* lv_obj_set_style_text_color(btnm, lv_color_hex(0xFFFFFF), LV_PART_ITEMS | LV_STATE_DEFAULT); */

    lv_obj_add_event_cb(btnm, pin_btn_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

/**
 * 按钮事件回调
 */
static void pin_btn_event_cb(lv_event_t * e)
{
    lv_obj_t * btnm = lv_event_get_target(e);
    const char * txt = lv_btnmatrix_get_btn_text(btnm, lv_btnmatrix_get_selected_btn(btnm));

    if (strcmp(txt, "DEL") == 0) {
        size_t len = strlen(pin_code);
        if (len > 0) {
            pin_code[len - 1] = '\0';
        }
    } else if (strcmp(txt, "OK") == 0) {
        LV_LOG_USER("PIN code entered: %s", pin_code);
        // 这里可以添加提交PIN码的处理代码
    } else {
        if (strlen(pin_code) < 6) {
            strcat(pin_code, txt);
        }
    }

    lv_label_set_text(pin_label, pin_code);
}
