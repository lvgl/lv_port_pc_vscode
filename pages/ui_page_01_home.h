#ifndef __UI_PAGE_01_HOME_H__
#define __UI_PAGE_01_HOME_H__

#include "lvgl.h"

lv_obj_t * ui_page_create_01_home(lv_obj_t *parent);

#define UI_NAME_STATE_HOME          "main.home"
void ui_after_create_state_home(const char * name, lv_obj_t * obj);

#endif
