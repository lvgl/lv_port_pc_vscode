#ifndef __IOC_THEME_MNGR_H__
#define __IOC_THEME_MNGR_H__

#include "lvgl.h"

#define THEME_ROOT_PATH "./assets/theme/"
#define THEME_PATCH_PATH "./patch/theme/"

typedef struct
{
    const char * name;
    const char * value;
} ioc_theme_color_t;

typedef struct
{
    const char * name;
#if (LVGL_VERSION_MAJOR == 9)
    lv_font_t * font;
#else
    lv_ft_info_t font_info;
#endif
} ioc_theme_font_t;

typedef struct {
    const char * name;
    const char * path;
} ioc_theme_img_t;

typedef struct {
    const char * name;

    ioc_theme_color_t * colors;
    int color_count;

    ioc_theme_font_t * fonts;
    int font_count;

    ioc_theme_img_t * imgs;
    int img_count;

} ioc_theme_t;


bool ioc_theme_mngr_load(const char * using_theme_name);
void ioc_theme_mngr_unload(void);
bool ioc_theme_mngr_switch(const char * using_theme_name);

lv_color_t ioc_theme_getcolor(const char * name);
const lv_font_t * ioc_theme_getfont(const char * name);
const char * ioc_theme_getimgpath(const char * name);

#endif
