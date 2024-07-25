/*********************
 *      INCLUDES
 *********************/
#ifndef __SETTINGS_ABOUT_H__
#define __SETTINGS_ABOUT_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} settings_about_t;

extern void settings_about_start(void);
extern void settings_about_stop(void);

#endif /* __SETTINGS_ABOUT_H__ */


