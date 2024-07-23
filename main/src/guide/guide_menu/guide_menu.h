/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_MENU_H__
#define __GUIDE_MENU_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} guide_menu_t;


extern void guide_menu_start(void);
extern void guide_menu_stop(void);


#endif /* __GUIDE_MENU_H__ */


