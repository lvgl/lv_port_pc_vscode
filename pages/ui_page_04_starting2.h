#ifndef __UI_PAGE_04_STARTING2_H__
#define __UI_PAGE_04_STARTING2_H__


#include "lvgl.h"

lv_obj_t * ui_page_create_04_starting2(lv_obj_t *parent);

#define UI_NAME_04_STARTING2     "main.starting2"
void ui_after_create_04_starting2(const char * name, lv_obj_t * obj);
uint8_t ui_page_04_starting2_get_autu_conidx(void);
#endif
