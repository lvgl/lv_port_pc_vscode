/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_GUIDE_H__
#define __GUIDE_GUIDE_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} guide_guide_t;



extern void guide_guide_start(void);
extern void guide_guide_stop(void);

#endif /* __GUIDE_GUIDE_H__ */


