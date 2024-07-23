/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_QUICK_START_H__
#define __GUIDE_QUICK_START_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} guide_quick_start_t;

extern void guide_quick_start_start(void);
extern void guide_quick_start_stop(void);


#endif /* __GUIDE_QUICK_START_H__ */


