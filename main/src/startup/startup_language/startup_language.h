/*********************
 *      INCLUDES
 *********************/
#ifndef __STARTUP_LANGUAGE_H__
#define __STARTUP_LANGUAGE_H__


#include <lvgl.h>
#include "gui_comm.h"


typedef struct
{
	app_index_t app_index;
	lv_obj_t* bg_cont;
} startup_language_t;


/*page_mode : page_language_mode_t*/
extern void startup_language_start(app_index_t app_index);
extern void startup_language_stop(void);

#endif /* __STARTUP_LANGUAGE_H__ */


