/*********************
 *      INCLUDES
 *********************/
#ifndef __CONNECT_MAIN_H__
#define __CONNECT_MAIN_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} connect_main_t;

extern void connect_main_start(void);
extern void connect_main_stop(void);

#endif /* __CONNECT_MAIN_H__ */


