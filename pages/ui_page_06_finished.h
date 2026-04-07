#ifndef __UI_PAGE_06_FINISHED_H__
#define __UI_PAGE_06_FINISHED_H__

#include "lvgl.h"

lv_obj_t * ui_page_create_06_finished(lv_obj_t *parent);

#define UI_NAME_06_FINISHED      "main.finished"
void ui_after_create_06_finished(const char * name, lv_obj_t * obj);
void ui_page_show_06_finished(void);
void ui_page_hide_06_finished(void);

#define UI_FINISHED_DATA  g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.finished
#define UI_FINISHED_SHOWE_PRICE g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].showPrice

#endif
