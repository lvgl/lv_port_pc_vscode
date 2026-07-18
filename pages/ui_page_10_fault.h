#ifndef __UI_PAGE_10_FAULT_H__
#define __UI_PAGE_10_FAULT_H__

#include "lvgl.h"

lv_obj_t * ui_page_create_10_fault(lv_obj_t *parent);

#define UI_NAME_10_FAULT              "main.fault"
void ui_after_create_10_fault(const char * name, lv_obj_t * obj);

#endif
