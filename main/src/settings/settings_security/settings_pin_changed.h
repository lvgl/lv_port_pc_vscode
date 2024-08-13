/*********************
 *      INCLUDES
 *********************/
#ifndef __SETTINGS_PIN_CHANGED_H__
#define __SETTINGS_PIN_CHANGED_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} settings_pin_changed_t;


extern void settings_pin_changed_start(void);
extern void settings_pin_changed_stop(void);

#endif /* __SETTINGS_PIN_CHANGED_H__ */


