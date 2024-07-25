/*********************
 *      INCLUDES
 *********************/
#ifndef __STARTUP_READY_CHECK_H__
#define __STARTUP_READY_CHECK_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} startup_ready_check_t;


extern void startup_ready_check_start(void);
extern void startup_ready_check_stop(void);

#endif /* __STARTUP_READY_CHECK_H__ */


