/*********************
 *      INCLUDES
 *********************/
#ifndef __STARTUP_QUICK_START_H__
#define __STARTUP_QUICK_START_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} startup_quick_start_t;

extern void startup_quick_start_start(void);
extern void startup_quick_start_stop(void);


#endif /* __STARTUP_QUICK_START_H__ */


