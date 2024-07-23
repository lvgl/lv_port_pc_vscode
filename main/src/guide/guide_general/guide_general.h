/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_GENERAL_H__
#define __GUIDE_GENERAL_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
	lv_obj_t* label_describe1;
	lv_obj_t* label_describe2;

	uint8_t page_id;
} guide_general_t;


void guide_general_start(void);
void guide_general_stop(void);

#endif /* __GUIDE_GENERAL_H__ */


