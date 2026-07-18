#ifndef __UI_PAGE_PRICE_CHARGING_H__
#define __UI_PAGE_PRICE_CHARGING_H__

#include "lvgl.h"

lv_obj_t * ui_page_create_price_charging(lv_obj_t *parent);

// void ui_page_show_price_charging(void);
// void ui_page_hide_price_charging(void);


#define UI_NAME_PRICE_CHARGING         "main.price_charging"
void ui_after_create_price_charging(const char * name, lv_obj_t * obj);

#endif
