/*********************
 *      INCLUDES
 *********************/
#ifndef __STARTUP_DONE_H__
#define __STARTUP_DONE_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} startup_done_t;

extern void startup_done_start(void);
extern void startup_done_stop(void);


#endif /* __STARTUP_DONE_H__ */


