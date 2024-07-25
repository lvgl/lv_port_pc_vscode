/*********************
 *      INCLUDES
 *********************/
#ifndef __STARTUP_LANGUAGE_H__
#define __STARTUP_LANGUAGE_H__


#include <lvgl.h>
#include "gui_comm.h"

typedef struct
{
	app_index_t app_index;

	lv_obj_t* bg_cont;
	lv_obj_t* pin_label;
	char pin[8];
} startup_enter_pin_t;


extern void startup_enter_pin_start(app_index_t app_index);
extern void startup_enter_pin_stop(void);


#endif /* __STARTUP_LANGUAGE_H__ */


