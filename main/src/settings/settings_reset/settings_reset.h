/*********************
 *      INCLUDES
 *********************/
#ifndef __SETTINGS_RESET_H__
#define __SETTINGS_RESET_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} settings_reset_t;

extern void settings_reset_start(void);
extern void settings_reset_stop(void);

#endif /* __SETTINGS_RESET_H__ */


