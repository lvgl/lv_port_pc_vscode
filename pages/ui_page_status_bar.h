#ifndef __UI_PAGE_STATUS_BAR_H__
#define __UI_PAGE_STATUS_BRA_H__

#include "lvgl.h"

typedef struct
{
    lv_obj_t *help_btn;
    lv_obj_t *help_text;
    lv_obj_t *lang_btn;
    lv_obj_t *lang_icon;
    lv_obj_t *lang_label;
    lv_obj_t *time;

} ui_status_bar_t;

#define UI_NAME_STATUS_BAR         "main.status_bar"
void ui_after_create_status_bar(const char * name, lv_obj_t * obj);

lv_obj_t * ui_page_create_status_bar(lv_obj_t *parent);
extern  ui_status_bar_t ui_status_bar;

#endif
