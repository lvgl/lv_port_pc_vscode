#ifndef __IOC_STYLE_MNGR_H__
#define __IOC_STYLE_MNGR_H__

#include "lvgl.h"

#define STYLE_ROOT_PATH "./assets/style/"
#define STYLE_PATCH_PATH "./patch/style/"

typedef struct
{
    const char * name;
    lv_style_t style;
} ioc_style_one_t;

typedef struct {
    char *name;

    ioc_style_one_t * styles;
    int style_count;

} ioc_style_t;

bool ioc_style_mngr_load(const char * using_style_name);
void ioc_style_mngr_unload(void);
bool ioc_style_mngr_switch(const char * using_style_name);
void ioc_style_mngr_theme_changed(void);

lv_style_t * ioc_style_mngr_get_style(const char *style_name);

#endif
