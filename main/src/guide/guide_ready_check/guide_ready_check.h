/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_READY_CHECK_H__
#define __GUIDE_READY_CHECK_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} guide_ready_check_t;


extern void guide_ready_check_start(void);
extern void guide_ready_check_stop(void);

#endif /* __GUIDE_READY_CHECK_H__ */


