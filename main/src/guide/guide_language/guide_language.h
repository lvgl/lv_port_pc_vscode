/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_LANGUAGE_H__
#define __GUIDE_LANGUAGE_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} guide_language_t;


extern void guide_language_start(void);
extern void guide_language_stop(void);

#endif /* __GUIDE_LANGUAGE_H__ */


