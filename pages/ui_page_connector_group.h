#ifndef __UI_PAGE_CONNECTOR_GROUP_H__
#define __UI_PAGE_CONNECTOR_GROUP_H__

#include "lvgl.h"

// 分别对应设置四种枪的对象(目前设计单枪不需要卡片)
// #define UI_NAME_CONNECTOR_CARD_SINGLE_PREFIX    "main.connector_single.connector*"
#define UI_NAME_CONNECTOR_CARD_DUAL_PREFIX      "main.conn_card_dual.connector*"
#define UI_NAME_CONNECTOR_CARD_TRIP_PREFIX      "main.conn_card_trip.connector*"
#define UI_NAME_CONNECTOR_CARD_QUAD_PREFIX      "main.conn_card_quad.connector*"
void ui_after_create_connector_card(const char * name, lv_obj_t * obj);

lv_obj_t * ui_page_create_connector_group(lv_obj_t *parent);
char * ui_connector_get_text_time_value(int idx);
#endif
