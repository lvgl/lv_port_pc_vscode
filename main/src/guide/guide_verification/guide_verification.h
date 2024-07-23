/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_VERIFICATION_H__
#define __GUIDE_VERIFICATION_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
	lv_obj_t* label_describe1;
	lv_obj_t* label_describe2;

	uint8_t page_id;
} guide_verification_t;


void guide_verification_start(void);
void guide_verification_stop(void);

#endif /* __GUIDE_VERIFICATION_H__ */


