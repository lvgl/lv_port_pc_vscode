#ifndef __UI_PAGE_PRICE_FINISHED_H__
#define __UI_PAGE_PRICE_FINISHED_H__

#include "lvgl.h"

lv_obj_t * ui_page_create_price_finished(lv_obj_t *parent);

#define UI_NAME_PRICE_FINISHED      "main.price_finished"
void ui_after_create_price_finished(const char * name, lv_obj_t * obj);

// void ui_page_show_price_finished(void);
// void ui_page_hide_price_finished(void);


#endif
