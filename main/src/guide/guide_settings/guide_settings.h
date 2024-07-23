/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_SETTINGS_H__
#define __GUIDE_SETTINGS_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} guide_settings_t;


extern void guide_settings_start(void);
extern void guide_settings_stop(void);

#endif /* __GUIDE_SETTINGS_H__ */


