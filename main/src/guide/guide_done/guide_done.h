/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_DONE_H__
#define __GUIDE_DONE_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} guide_done_t;

void guide_done_start(void);
void guide_done_stop(void);


#endif /* __GUIDE_DONE_H__ */


