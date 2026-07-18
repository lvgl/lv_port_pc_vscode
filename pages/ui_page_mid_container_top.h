#ifndef __UI_PAGE_MID_CONTAINER_TOP_H__
#define __UI_PAGE_MID_CONTAINER_TOP_H__

#include "lvgl.h"

lv_obj_t * ui_page_create_mid_container_top(lv_obj_t *parent);

#define UI_NAME_STATE_MID_CONTAINER_TOP          "main.conn_top_info"
void ui_after_create_state_mid_container_top(const char * name, lv_obj_t * obj);

#endif
