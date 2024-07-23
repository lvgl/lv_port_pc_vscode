/*********************
 *      INCLUDES
 *********************/
#ifndef __GUIDE_SCREEN_H__
#define __GUIDE_SCREEN_H__


#include <lvgl.h>

typedef struct
{	
	lv_coord_t move_y;
	lv_obj_t* bg_cont;
	lv_obj_t* scroll_img;
	lv_obj_t* scroll_label;
} guide_screen_t;

extern void guide_screen_start(void);
extern void guide_screen_stop(void);


#endif /* __GUIDE_SCREEN_H__ */


