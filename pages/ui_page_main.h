#ifndef __UI_PAGE_MAIN_H__
#define __UI_PAGE_MAIN_H__

#include "lvgl.h"

typedef enum {
    UI_PAGE_HOME = 0,
    UI_PAGE_IDLE,
    UI_PAGE_STARTING1,
    UI_PAGE_STARTING2,
    UI_PAGE_CHARTGING,
    UI_PAGE_FINISHED,
    UI_PAGE_RESERVED,
    UI_PAGE_SESSION_ERROR,
    UI_PAGE_START_ERROR,
    UI_PAGE_FAULT,
    UI_PAGE_UNAVAILABLE,
    UI_PAGE_CONNECTING,
    UI_PAGE_STOPPING,
    UI_PAGE_WAIT_IN_LINE,
    UI_PAGE_SUSPEND,
    UI_PAGE_PROCESSING,

    UI_PAGE_MAX,
} ioc_ui_page_e;

lv_obj_t * ui_page_main_create(lv_obj_t *parent);

#endif
