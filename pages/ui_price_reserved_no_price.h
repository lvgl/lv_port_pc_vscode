#ifndef __UI_PAGE_PRICE_RESERVED_NO_PRICE_H__
#define __UI_PAGE_PRICE_RESERVED_NO_PRICE_H__


#include "lvgl.h"

lv_obj_t * ui_page_create_price_reserved_no_price(lv_obj_t *parent);
void ui_after_create_price_reserved_no_price(const char * name, lv_obj_t * obj);


#define UI_NAME_PRICE_RESERVED_NO_PRICE      "main.price_reserved_no_price"

// void ui_page_show_price_reserved_no_price(void);
// void ui_page_hide_price_reserved_no_price(void);

#endif
