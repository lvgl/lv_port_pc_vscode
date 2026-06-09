#ifndef __UI_POPUP_PREAUTHORIZATION_H__
#define __UI_POPUP_PREAUTHORIZATION_H__


#include "lvgl.h"

#define UI_NAME_PRE_AUTHORIZATION         "pre_authorization"

// 弹出预授权提示框, 用户输入完毕确认后直接发送预授权请求给ui_json, 调用界面不需要处理预授权金额
void ui_popup_show_preauthorization(int idx);

void ui_after_create_preauthorization(const char * name, lv_obj_t * obj);

#endif
