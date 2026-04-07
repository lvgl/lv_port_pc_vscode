#ifndef __UI_POPUP_BILL_H__
#define __UI_POPUP_BILL_H__

#include "lvgl.h"

#define UI_NAME_BILL                      "bill"

// 显示某枪的账单弹出信息, 内容由ui_json提供
void ui_popup_show_bill();

void ui_popup_hide_bill();

void ui_after_create_bill(const char * name, lv_obj_t * obj);

#endif
