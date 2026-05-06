#ifndef __UI_PAGE_17_PROCESSING_H__
#define __UI_PAGE_17_PROCESSING_H__

#include "lvgl.h"

lv_obj_t * ui_page_create_17_processing(lv_obj_t *parent);

void ui_page_show_17_processing(void);
void ui_page_hide_17_processing(void);

#define UI_NAME_17_PROCESSING            "main.processing"
void ui_after_create_17_processing(const char * name, lv_obj_t * obj);

#endif
