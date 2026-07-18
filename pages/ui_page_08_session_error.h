#ifndef __UI_PAGE_08_SESSION_ERROR_H__
#define __UI_PAGE_08_SESSION_ERROR_H__

#include "lvgl.h"

lv_obj_t * ui_page_create_08_session_error(lv_obj_t *parent);

#define UI_NAME_08_SESSION_ERROR      "main.session_error"
void ui_after_create_08_session_error(const char * name, lv_obj_t * obj);

#endif
