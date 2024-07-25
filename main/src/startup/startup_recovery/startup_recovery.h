/*********************
 *      INCLUDES
 *********************/
#ifndef __STARTUP_RECOVERY_H__
#define __STARTUP_RECOVERY_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} startup_recovery_t;

extern void startup_recovery_start(void);
extern void startup_recovery_stop(void);

#endif /* __STARTUP_RECOVERY_H__ */


