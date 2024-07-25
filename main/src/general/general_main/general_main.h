/*********************
 *      INCLUDES
 *********************/
#ifndef __GENERAL_MAIN_H__
#define __GENERAL_MAIN_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
	lv_obj_t* label_describe1;
	lv_obj_t* label_describe2;

	uint8_t page_id;
} general_main_t;


extern void general_main_start(void);
extern void general_main_stop(void);

#endif /* __GENERAL_MAIN_H__ */


