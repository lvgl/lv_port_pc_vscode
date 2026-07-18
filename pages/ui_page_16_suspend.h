#ifndef __UI_PAGE_16_SUSPEND_H__
#define __UI_PAGE_16_SUSPEND_H__

#include "lvgl.h"

lv_obj_t * ui_page_create_16_suspend(lv_obj_t *parent);

#define UI_NAME_16_SUSPEND            "main.suspend"
void ui_after_create_16_suspend(const char * name, lv_obj_t * obj);

#endif
