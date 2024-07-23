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


void guide_language_start(void);
void guide_language_stop(void);

#endif /* __GUIDE_LANGUAGE_H__ */


