/*********************
 *      INCLUDES
 *********************/
#ifndef __GENERAL_HOMESCREEN_H__
#define __GENERAL_HOMESCREEN_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
	lv_obj_t* img_choosen;
	
	void* bg_img_src_choosen;
} general_homescreen_t;


extern void general_homescreen_start(void);
extern void general_homescreen_stop(void);

#endif /* __GENERAL_HOMESCREEN_H__ */


