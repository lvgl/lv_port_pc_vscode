/*********************
 *      INCLUDES
 *********************/
#ifndef __SETTINGS_ABOUT_H__
#define __SETTINGS_ABOUT_H__


#include <rtthread.h>
#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
	lv_timer_t* timer;
} settings_about_t;


void settings_about_create_timer();
void settings_about_refresh(struct _lv_timer_t* task);

#endif /* __SETTINGS_ABOUT_H__ */


