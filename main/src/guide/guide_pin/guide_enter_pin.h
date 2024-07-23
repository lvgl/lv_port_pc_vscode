/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_LANGUAGE_H__
#define __GUIDE_LANGUAGE_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
	lv_obj_t* pin_label;
	char pin[8];
} guide_enter_pin_t;


extern void guide_enter_pin_start(void);
extern void guide_enter_pin_stop(void);


#endif /* __GUIDE_LANGUAGE_H__ */


