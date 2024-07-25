/*********************
 *      INCLUDES
 *********************/
#ifndef __SETTINGS_MAIN_H__
#define __SETTINGS_MAIN_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} settings_main_t;


extern void settings_main_start(void);
extern void settings_main_stop(void);

#endif /* __SETTINGS_MAIN_H__ */


