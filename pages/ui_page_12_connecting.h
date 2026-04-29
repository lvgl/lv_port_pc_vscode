#ifndef __UI_PAGE_12_CONNECTING_H__
#define __UI_PAGE_12_CONNECTING_H__


#include "lvgl.h"

lv_obj_t * ui_page_create_12_connecting(lv_obj_t *parent);

#define UI_NAME_12_CONNECTING         "main.connecting"
void ui_after_create_12_connecting(const char * name, lv_obj_t * obj);

#endif
