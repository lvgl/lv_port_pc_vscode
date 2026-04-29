#ifndef __IOC_FRAGMENT_MNGR_H__
#define __IOC_FRAGMENT_MNGR_H__

#include "lvgl.h"

#define FRAGMENT_ROOT_PATH "./assets/fragment/"
#define FRAGMENT_PATCH_PATH "./patch/fragment/"

bool ioc_fragment_mngr_load(void);
void ioc_fragment_mngr_unload(void);

lv_obj_t * ioc_obj_create_fragment(const char * name, cJSON * fragment_json, lv_obj_t * parent);


#endif
