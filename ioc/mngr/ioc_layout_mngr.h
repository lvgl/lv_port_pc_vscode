#ifndef __IOC_LAYOUT_MNGR_H__
#define __IOC_LAYOUT_MNGR_H__

#include "lvgl.h"

#define LAYOUT_ROOT_PATH "./assets/layout/"
#define LAYOUT_PATCH_PATH "./patch/layout/"

bool ioc_layout_mngr_load(void);
void ioc_layout_mngr_unload(void);

lv_obj_t * ioc_layout_mngr_load_layout(const char *layout_name, lv_obj_t * screen);

#endif
