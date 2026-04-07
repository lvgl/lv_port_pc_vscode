#ifndef __UI_PAGE_07_RESERVED_H__
#define __UI_PAGE_07_RESERVED_H__


#include "lvgl.h"

lv_obj_t * ui_page_create_07_reserved(lv_obj_t *parent);

#define UI_NAME_07_RESERVED      "main.reserved"
#define UI_RESERVED_DATA  g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.reserved
#define UI_RESERVED_SHOWE_PRICE g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].showPrice
#define UI_RESERVED_SHOWE_OCCPP_PRICE g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].isOcppPrice
void ui_after_create_07_reserved(const char * name, lv_obj_t * obj);
void ui_page_show_07_reserved(void);
void ui_page_hide_07_reserved(void);


#endif
