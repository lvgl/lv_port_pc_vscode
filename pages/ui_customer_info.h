#ifndef __UI_PAGE_CUSTOMER_INFO_H__
#define __UI_PAGE_CUSTOMER_INFO_H__

#include "lvgl.h"

#define UI_NAME_CUSTOMER_INFO          "main.customer_info"

lv_obj_t * ui_page_create_customer_info(lv_obj_t *parent);
void ui_after_create_customer_info(const char * name, lv_obj_t * obj);

#endif
