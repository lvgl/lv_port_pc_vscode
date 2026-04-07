#ifndef __UI_PAGE_15_WAIT_IN_LINE_H__
#define __UI_PAGE_15_WAIT_IN_LINE_H__


#include "lvgl.h"

lv_obj_t * ui_page_create_15_wait_in_line(lv_obj_t *parent);
void ui_page_show_15_wait_in_line(void);
void ui_page_hide_15_wait_in_line(void);

#define UI_NAME_15_WAIT_IN_LINE       "main.wait_in_line"
void ui_after_create_15_wait_in_line(const char * name, lv_obj_t * obj);

#endif
