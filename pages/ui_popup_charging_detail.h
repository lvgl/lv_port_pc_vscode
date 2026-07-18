#ifndef __UI_POPUP_CHARGING_DETAIL_H__
#define __UI_POPUP_CHARGING_DETAIL_H__

#include "lvgl.h"


#define UI_NAME_DETAILED_DATA             "detailed_data"

// 显示充电详情弹出信息, 内容由ui_json提供
void ui_popup_show_charging_detail(int idx, int type);

void ui_after_create_detaile_data(const char * name, lv_obj_t * obj);
#endif
