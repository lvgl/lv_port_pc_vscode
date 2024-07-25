/*********************
 *      INCLUDES
 *********************/
#ifndef __STARTUP_SET_PIN_H__
#define __STARTUP_SET_PIN_H__


#include <lvgl.h>
#include "gui_comm.h"

enum
{
	STARTUP_SET_PIN_MODE_SET = 0,
	STARTUP_SET_PIN_MODE_CONFIRM,
};
typedef uint8_t startup_set_pin_mode_t;

typedef struct
{
	lv_obj_t* bg_cont;
	lv_obj_t* title_label;
	lv_obj_t* pin_label;

	app_index_t app_index;
	startup_set_pin_mode_t mode;
	char set_pin[8];
	char confirm_pin[8];
} startup_set_pin_t;

extern void startup_set_pin_start(app_index_t app_index);
extern void startup_set_pin_stop(void);

#endif /* __STARTUP_SET_PIN_H__ */


