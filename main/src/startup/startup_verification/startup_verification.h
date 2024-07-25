/*********************
 *      INCLUDES
 *********************/
#ifndef __STARTUP_VERIFICATION_H__
#define __STARTUP_VERIFICATION_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
	lv_obj_t* label_describe1;
	lv_obj_t* label_describe2;

	uint8_t page_id;
} startup_verification_t;


void startup_verification_start(void);
void startup_verification_stop(void);

#endif /* __STARTUP_VERIFICATION_H__ */


