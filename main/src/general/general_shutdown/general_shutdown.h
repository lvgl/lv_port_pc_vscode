/*********************
 *      INCLUDES
 *********************/
#ifndef __GENERAL_SHUTDOWN_H__
#define __GENERAL_SHUTDOWN_H__


#include <lvgl.h>

typedef struct
{

	lv_obj_t* bg_cont;
	lv_obj_t* choosen_btn;
	lv_obj_t* label_describe;

	int shutdown_time;
} general_shutdown_t;


extern void general_shutdown_start(void);
extern void general_shutdown_stop(void);

#endif /* __GENERAL_SHUTDOWN_H__ */


