/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_SET_PIN_H__
#define __GUIDE_SET_PIN_H__


#include <lvgl.h>

enum
{
	GUIDE_SET_PIN_MODE_SET = 0,
	GUIDE_SET_PIN_MODE_CONFIRM,
};
typedef uint8_t guide_set_pin_mode_t;

typedef struct
{
	lv_obj_t* bg_cont;
	lv_obj_t* title_label;
	lv_obj_t* pin_label;

	guide_set_pin_mode_t mode;
	char set_pin[8];
	char confirm_pin[8];
} guide_set_pin_t;

extern void guide_set_pin_start(void);
extern void guide_set_pin_stop(void);

#endif /* __GUIDE_SET_PIN_H__ */


