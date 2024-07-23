/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_CONNECT_H__
#define __GUIDE_CONNECT_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} guide_connect_t;

void guide_connect_start(void);
void guide_connect_stop(void);

#endif /* __GUIDE_CONNECT_H__ */


