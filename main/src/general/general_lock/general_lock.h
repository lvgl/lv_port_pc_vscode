/*********************
 *      INCLUDES
 *********************/
#ifndef __GENERAL_LOCK_H__
#define __GENERAL_LOCK_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
	lv_obj_t* label_describe1;
	lv_obj_t* label_describe2;

	uint8_t page_id;
} general_lock_t;


extern void general_lock_start(void);
extern void general_lock_stop(void);

#endif /* __GENERAL_LOCK_H__ */


