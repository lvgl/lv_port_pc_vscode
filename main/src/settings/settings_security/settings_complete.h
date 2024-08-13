/*********************
 *      INCLUDES
 *********************/
#ifndef __SETTINGS_COMPLETE_H__
#define __SETTINGS_COMPLETE_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} settings_complete_t;

extern void settings_complete_start(void);
extern void settings_complete_stop(void);

#endif /* __SETTINGS_COMPLETE_H__ */


