#ifndef __UI_PAGE_PRICE_FINISHED_NO_PRICE_H__
#define __UI_PAGE_PRICE_FINISHED_NO_PRICE_H__

#include "lvgl.h"

lv_obj_t * ui_page_create_price_finished_no_price(lv_obj_t *parent);

#define UI_NAME_PRICE_FINISHED_NO_PRICE      "main.price_finished_no_price"
void ui_after_create_price_finished_no_price(const char * name, lv_obj_t * obj);

// void ui_page_show_price_finished(void);
// void ui_page_hide_price_finished(void);


#endif
