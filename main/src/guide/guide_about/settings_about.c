/*********************
 *      INCLUDES
 *********************/

#include "global.h"
#include "settings_about.h"
#include "settings_comm.h"

#define _MODULE_NAME_ "settings"
#include "app_module.h"

static settings_about_t* p_settings_about = NULL;

static void settings_about_bg_cont(lv_obj_t* parent)
{

}

static void on_start(void)
{
    p_settings_about = (settings_about_t*)lv_malloc(sizeof(settings_about_t));
    p_settings_about->bg_cont = settings_draw_bg_cont();
    settings_about_bg_cont(p_settings_about->bg_cont);
}

static void on_stop(void)
{
    lv_obj_del(p_settings_about->bg_cont);
    lv_free(p_settings_about);
    p_settings_about = NULL;
}

