#ifndef __UI_POPUP_LANGUAGE_H__
#define __UI_POPUP_LANGUAGE_H__

#include "lvgl.h"

#define UI_NAME_LANGUAGE "language"
void ui_after_create_language(const char * name, lv_obj_t * obj);

// 显示语言选择页面
void ui_popup_show_language(void);

void parse_all_lang_jsons(void);
void print_translations(const lv_i18n_lang_t *lang);

#define LANGUAGE_SIZE 255

typedef struct {
    char locale[32];     // 语言代码，如 "en-US"
    char label[64];      // 展示标签，如 "English"
    char icon[128];      // 图标路径
    char filename[128];  // 原始 JSON 文件名，便于定位
} lang_info_t;

typedef struct {
    lang_info_t info;
    lv_obj_t *btn_obj;  // 按钮对象
} lang_button_t;
typedef struct
{
    lv_obj_t * language;            // 语言选择页面

    lv_obj_t * close_btn;           // 关闭按钮
    lv_obj_t * confirm_btn;         // 确认按钮
    lv_obj_t * close_text;          // 关闭按钮文本
    lv_obj_t * confirm_text;        // 确认按钮文本
    lv_obj_t * language_label;      // 语言文本
    int lang_count;                 // 语言按钮个数
    lang_button_t *lang_buttons;    // 指向语言按钮数组
    bool check_language;            // 选择语言
    bool confirm_language;            // 选择语言
    char *selected_locale;
    lv_obj_t * selected_btn;
} ui_language_t;

extern ui_language_t s_ui_language;
void ui_language_update_selected(const char *str);
void language_page_init(void);
#endif
