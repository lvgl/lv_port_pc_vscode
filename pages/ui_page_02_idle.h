#ifndef __UI_PAGE_02_IDLE_H__
#define __UI_PAGE_02_IDLE_H__

#include "lvgl.h"

lv_obj_t * ui_page_create_02_idle(lv_obj_t *parent);

#define UI_NAME_02_IDLE          "main.idle"
void ui_after_create_02_idle(const char * name, lv_obj_t * obj);

#endif
