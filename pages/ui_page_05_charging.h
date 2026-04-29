#ifndef __UI_PAGE_05_CHARGING_H__
#define __UI_PAGE_05_CHARGING_H__

#include "lvgl.h"

lv_obj_t * ui_page_create_05_charging(lv_obj_t *parent);
void ui_page_show_05_charging(void);
void ui_page_hide_05_charging(void);


#define UI_NAME_05_CHARGING         "main.charging"
void ui_after_create_05_charging(const char * name, lv_obj_t * obj);

#endif
