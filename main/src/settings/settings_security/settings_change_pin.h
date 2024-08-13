/*********************
 *      INCLUDES
 *********************/
#ifndef __SETTINGS_CHANGE_PIN_H__
#define __SETTINGS_CHANGE_PIN_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} settings_change_pin_t;


extern void settings_change_pin_start(void);
extern void settings_change_pin_stop(void);

#endif /* __SETTINGS_CHANGE_PIN_H__ */


