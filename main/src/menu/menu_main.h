/*********************
 *      INCLUDES
 *********************/
#ifndef __MENU_MAIN_H__
#define __MENU_MAIN_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} menu_main_t;


extern void menu_main_start(void);
extern void menu_main_stop(void);


#endif /* __MENU_MAIN_H__ */


