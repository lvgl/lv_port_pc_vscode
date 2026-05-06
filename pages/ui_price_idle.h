#ifndef __UI_PRICE_IDLE_H__
#define __UI_PRICE_IDLE_H__

#include "lvgl.h"

lv_obj_t * ui_page_create_price_idle(lv_obj_t *parent);

#define UI_NAME_PRICE_IDLE          "main.price_idle"
void ui_after_create_price_idle(const char * name, lv_obj_t * obj);

#endif
