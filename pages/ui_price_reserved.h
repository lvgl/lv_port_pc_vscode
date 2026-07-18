#ifndef __UI_PAGE_PRICE_RESERVED_H__
#define __UI_PAGE_PRICE_RESERVED_H__


#include "lvgl.h"

lv_obj_t * ui_page_create_price_reserved(lv_obj_t *parent);
void ui_after_create_price_reserved(const char * name, lv_obj_t * obj);


#define UI_NAME_PRICE_RESERVED      "main.price_reserved"

// void ui_page_show_price_reserved(void);
// void ui_page_hide_price_reserved(void);


#endif
