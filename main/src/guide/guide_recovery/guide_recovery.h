/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_RECOVERY_H__
#define __GUIDE_RECOVERY_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} guide_recovery_t;

extern void guide_recovery_start(void);
extern void guide_recovery_stop(void);

#endif /* __GUIDE_RECOVERY_H__ */


