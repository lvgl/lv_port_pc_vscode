#ifndef __UI_POPUP_HELP_H__
#define __UI_POPUP_HELP_H__

#include "lvgl.h"

#define HELP_JSON_FILE  "./assets/help.json"
#define HELP_PATCH_FILE  "./patch/help.json"

#define UI_NAME_HELP1 "help1"
void ui_after_create_help1(const char * name, lv_obj_t * obj);

#define UI_NAME_HELP2 "help2"
void ui_after_create_help2(const char * name, lv_obj_t * obj);

// 显示帮助页面
void ui_popup_show_help(void);


#endif
