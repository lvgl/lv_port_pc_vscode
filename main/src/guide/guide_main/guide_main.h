/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_MAIN_H__
#define __GUIDE_MAIN_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} guide_main_t;



extern void guide_main_start(void);
extern void guide_main_stop(void);

#endif /* __GUIDE_MAIN_H__ */


