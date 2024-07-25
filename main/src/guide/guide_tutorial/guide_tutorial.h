/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_TUTORIAL_H__
#define __GUIDE_TUTORIAL_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} guide_tutorial_t;



extern void guide_tutorial_start(void);
extern void guide_tutorial_stop(void);

#endif /* __GUIDE_TUTORIAL_H__ */


